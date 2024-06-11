/* Copyright (c) 2000, 2011, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

/*
  This file defines the NDB Cluster handler engine_condition_pushdown
*/

#ifdef USE_PRAGMA_IMPLEMENTATION
#pragma implementation				// gcc: Class implementation
#endif

#include "sql_priv.h"
#include "sql_class.h"                          // set_var.h: THD
#include "my_global.h"                          // WITH_*
#include "log.h"                                // sql_print_error

#ifdef WITH_NDBCLUSTER_STORAGE_ENGINE
#include <ndbapi/NdbApi.hpp>
#include "ha_ndbcluster_cond.h"

// Typedefs for long names 
typedef NdbDictionary::Column NDBCOL;
typedef NdbDictionary::Table NDBTAB;


/**
  Serialize a constant item into a Ndb_cond node.

  @param  const_type  item's result type
  @param  item        item to be serialized
  @param  curr_cond   Ndb_cond node the item to be serialized into
  @param  context     Traverse context
*/

static void ndb_serialize_const(Item_result const_type, const Item *item,
                                Ndb_cond *curr_cond,
                                Ndb_cond_traverse_context *context)
{
  DBUG_ASSERT(item->const_item());
  switch (const_type) {
  case STRING_RESULT:
  {
    NDB_ITEM_QUALIFICATION q;
    q.value_type= Item::STRING_ITEM;
    curr_cond->ndb_item= new Ndb_item(NDB_VALUE, q, item); 
    if (! context->expecting_no_field_result())
    {
      // We have not seen the field argument yet
      context->expect_only(Item::FIELD_ITEM);
      context->expect_only_field_result(STRING_RESULT);
      context->expect_collation(item->collation.collation);
    }
    else
    {
      // Expect another logical expression
      context->expect_only(Item::FUNC_ITEM);
      context->expect(Item::COND_ITEM);
      // Check that string result have correct collation
      if (!context->expecting_collation(item->collation.collation))
      {
        DBUG_PRINT("info", ("Found non-matching collation %s",  
                            item->collation.collation->name));
        context->supported= FALSE;
      }
    }
    break;
  }
  case REAL_RESULT:
  {
    NDB_ITEM_QUALIFICATION q;
    q.value_type= Item::REAL_ITEM;
    curr_cond->ndb_item= new Ndb_item(NDB_VALUE, q, item);
    if (! context->expecting_no_field_result()) 
    {
      // We have not seen the field argument yet
      context->expect_only(Item::FIELD_ITEM);
      context->expect_only_field_result(REAL_RESULT);
    }
    else
    {
      // Expect another logical expression
      context->expect_only(Item::FUNC_ITEM);
      context->expect(Item::COND_ITEM);
    }
    break;
  }
  case INT_RESULT:
  {
    NDB_ITEM_QUALIFICATION q;
    q.value_type= Item::INT_ITEM;
    curr_cond->ndb_item= new Ndb_item(NDB_VALUE, q, item);
    if (! context->expecting_no_field_result()) 
    {
      // We have not seen the field argument yet
      context->expect_only(Item::FIELD_ITEM);
      context->expect_only_field_result(INT_RESULT);
    }
    else
    {
      // Expect another logical expression
      context->expect_only(Item::FUNC_ITEM);
      context->expect(Item::COND_ITEM);
    }
    break;
  }
  case DECIMAL_RESULT:
  {
    NDB_ITEM_QUALIFICATION q;
    q.value_type= Item::DECIMAL_ITEM;
    curr_cond->ndb_item= new Ndb_item(NDB_VALUE, q, item);
    if (! context->expecting_no_field_result()) 
    {
      // We have not seen the field argument yet
      context->expect_only(Item::FIELD_ITEM);
      context->expect_only_field_result(DECIMAL_RESULT);
    }
    else
    {
      // Expect another logical expression
      context->expect_only(Item::FUNC_ITEM);
      context->expect(Item::COND_ITEM);
    }
    break;
  }
  default:
    break;
  }
}
/*
  Serialize the item tree into a linked list represented by Ndb_cond
  for fast generation of NbdScanFilter. Adds information such as
  position of fields that is not directly available in the Item tree.
  Also checks if condition is supported.
*/
void ndb_serialize_cond(const Item *item, void *arg)
{
  Ndb_cond_traverse_context *context= (Ndb_cond_traverse_context *) arg;
  DBUG_ENTER("ndb_serialize_cond");  

  // Check if we are skipping arguments to a function to be evaluated
  if (context->skip)
  {
    if (!item)
    {
      DBUG_PRINT("info", ("Unexpected mismatch of found and expected number of function arguments %u", context->skip));
      sql_print_error("ndb_serialize_cond: Unexpected mismatch of found and "
                      "expected number of function arguments %u", context->skip);
      context->skip= 0;
      DBUG_VOID_RETURN;
    }
    DBUG_PRINT("info", ("Skiping argument %d", context->skip));
    context->skip--;
    switch (item->type()) {
    case Item::FUNC_ITEM:
    {
      Item_func *func_item= (Item_func *) item;
      context->skip+= func_item->argument_count();
      break;
    }
    case Item::INT_ITEM:
    case Item::REAL_ITEM:
    case Item::STRING_ITEM:
    case Item::VARBIN_ITEM:
    case Item::DECIMAL_ITEM:
      break;
    default:
      context->supported= FALSE;
      break;
    }
    
    DBUG_VOID_RETURN;
  }
  
  if (context->supported)
  {
    Ndb_rewrite_context *rewrite_context2= context->rewrite_stack;
    const Item_func *rewrite_func_item;
    // Check if we are rewriting some unsupported function call
    if (rewrite_context2 &&
        (rewrite_func_item= rewrite_context2->func_item) &&
        rewrite_context2->count++ == 0)
    {
      switch (rewrite_func_item->functype()) {
      case Item_func::BETWEEN:
        /*
          Rewrite 
          <field>|<const> BETWEEN <const1>|<field1> AND <const2>|<field2>
          to <field>|<const> > <const1>|<field1> AND 
          <field>|<const> < <const2>|<field2>
          or actually in prefix format
          BEGIN(AND) GT(<field>|<const>, <const1>|<field1>), 
          LT(<field>|<const>, <const2>|<field2>), END()
        */
      case Item_func::IN_FUNC:
      {
        /*
          Rewrite <field>|<const> IN(<const1>|<field1>, <const2>|<field2>,..)
          to <field>|<const> = <const1>|<field1> OR 
          <field> = <const2>|<field2> ...
          or actually in prefix format
          BEGIN(OR) EQ(<field>|<const>, <const1><field1>), 
          EQ(<field>|<const>, <const2>|<field2>), ... END()
          Each part of the disjunction is added for each call
          to ndb_serialize_cond and end of rewrite statement 
          is wrapped in end of ndb_serialize_cond
        */
        if (context->expecting(item->type()) || item->const_item())
        {
          // This is the <field>|<const> item, save it in the rewrite context
          rewrite_context2->left_hand_item= item;
          if (item->type() == Item::FUNC_ITEM)
          {
            Item_func *func_item= (Item_func *) item;
            if ((func_item->functype() == Item_func::UNKNOWN_FUNC ||
                 func_item->functype() == Item_func::NEG_FUNC) &&
                func_item->const_item())
            {
              // Skip any arguments since we will evaluate function instead
              DBUG_PRINT("info", ("Skip until end of arguments marker"));
              context->skip= func_item->argument_count();
            }
            else
            {
              DBUG_PRINT("info", ("Found unsupported functional expression in BETWEEN|IN"));
              context->supported= FALSE;
              DBUG_VOID_RETURN;
              
            }
          }
        }
        else
        {
          // Non-supported BETWEEN|IN expression
          DBUG_PRINT("info", ("Found unexpected item of type %u in BETWEEN|IN",
                              item->type()));
          context->supported= FALSE;
          DBUG_VOID_RETURN;
        }
        break;
      }
      default:
        context->supported= FALSE;
        break;
      }
      DBUG_VOID_RETURN;
    }
    else
    {
      Ndb_cond_stack *ndb_stack= context->stack_ptr;
      Ndb_cond *prev_cond= context->cond_ptr;
      Ndb_cond *curr_cond= context->cond_ptr= new Ndb_cond();
      if (!ndb_stack->ndb_cond)
        ndb_stack->ndb_cond= curr_cond;
      curr_cond->prev= prev_cond;
      if (prev_cond) prev_cond->next= curr_cond;
    // Check if we are rewriting some unsupported function call
      if (context->rewrite_stack)
      {
        Ndb_rewrite_context *rewrite_context= context->rewrite_stack;
        const Item_func *func_item= rewrite_context->func_item;
        switch (func_item->functype()) {
        case Item_func::BETWEEN:
        {
          /*
            Rewrite 
            <field>|<const> BETWEEN <const1>|<field1> AND <const2>|<field2>
            to <field>|<const> > <const1>|<field1> AND 
            <field>|<const> < <const2>|<field2>
            or actually in prefix format
            BEGIN(AND) GT(<field>|<const>, <const1>|<field1>), 
            LT(<field>|<const>, <const2>|<field2>), END()
          */
          if (rewrite_context->count == 2)
          {
            // Lower limit of BETWEEN
            DBUG_PRINT("info", ("GE_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(Item_func::GE_FUNC, 2);
          }
          else if (rewrite_context->count == 3)
          {
            // Upper limit of BETWEEN
            DBUG_PRINT("info", ("LE_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(Item_func::LE_FUNC, 2);
          }
          else
          {
            // Illegal BETWEEN expression
            DBUG_PRINT("info", ("Illegal BETWEEN expression"));
            context->supported= FALSE;
            DBUG_VOID_RETURN;
          }
          break;
        }
        case Item_func::IN_FUNC:
        {
          /*
            Rewrite <field>|<const> IN(<const1>|<field1>, <const2>|<field2>,..)
            to <field>|<const> = <const1>|<field1> OR 
            <field> = <const2>|<field2> ...
            or actually in prefix format
            BEGIN(OR) EQ(<field>|<const>, <const1><field1>), 
            EQ(<field>|<const>, <const2>|<field2>), ... END()
            Each part of the disjunction is added for each call
            to ndb_serialize_cond and end of rewrite statement 
            is wrapped in end of ndb_serialize_cond
          */
          DBUG_PRINT("info", ("EQ_FUNC"));      
          curr_cond->ndb_item= new Ndb_item(Item_func::EQ_FUNC, 2);
          break;
        }
        default:
          context->supported= FALSE;
        }
        // Handle left hand <field>|<const>
        context->rewrite_stack= NULL; // Disable rewrite mode
        context->expect_only(Item::FIELD_ITEM);
        context->expect_field_result(STRING_RESULT);
        context->expect_field_result(REAL_RESULT);
        context->expect_field_result(INT_RESULT);
        context->expect_field_result(DECIMAL_RESULT);
        context->expect(Item::INT_ITEM);
        context->expect(Item::STRING_ITEM);
        context->expect(Item::VARBIN_ITEM);
        context->expect(Item::FUNC_ITEM);
        ndb_serialize_cond(rewrite_context->left_hand_item, arg);
        context->skip= 0; // Any FUNC_ITEM expression has already been parsed
        context->rewrite_stack= rewrite_context; // Enable rewrite mode
        if (!context->supported)
          DBUG_VOID_RETURN;

        prev_cond= context->cond_ptr;
        curr_cond= context->cond_ptr= new Ndb_cond();
        prev_cond->next= curr_cond;
      }
      
      // Check for end of AND/OR expression
      if (!item)
      {
        // End marker for condition group
        DBUG_PRINT("info", ("End of condition group"));
        curr_cond->ndb_item= new Ndb_item(NDB_END_COND);
      }
      else
      {
        switch (item->type()) {
        case Item::FIELD_ITEM:
        {
          Item_field *field_item= (Item_field *) item;
          Field *field= field_item->field;
          enum_field_types type= field->type();
          /*
            Check that the field is part of the table of the handler
            instance and that we expect a field with of this result type.
          */
          if (context->table->s == field->table->s)
          {       
            const NDBTAB *tab= context->ndb_table;
            DBUG_PRINT("info", ("FIELD_ITEM"));
            DBUG_PRINT("info", ("table %s", tab->getName()));
            DBUG_PRINT("info", ("column %s", field->field_name));
            DBUG_PRINT("info", ("type %d", field->type()));
            DBUG_PRINT("info", ("result type %d", field->result_type()));
            
            // Check that we are expecting a field and with the correct
            // result type
            if (context->expecting(Item::FIELD_ITEM) &&
                context->expecting_field_type(field->type()) &&
                (context->expecting_field_result(field->result_type()) ||
                 // Date and year can be written as string or int
                 ((type == MYSQL_TYPE_TIME ||
                   type == MYSQL_TYPE_DATE || 
                   type == MYSQL_TYPE_YEAR ||
                   type == MYSQL_TYPE_DATETIME)
                  ? (context->expecting_field_result(STRING_RESULT) ||
                     context->expecting_field_result(INT_RESULT))
                  : TRUE)) &&
                // Bit fields no yet supported in scan filter
                type != MYSQL_TYPE_BIT &&
                // No BLOB support in scan filter
                type != MYSQL_TYPE_TINY_BLOB &&
                type != MYSQL_TYPE_MEDIUM_BLOB &&
                type != MYSQL_TYPE_LONG_BLOB &&
                type != MYSQL_TYPE_BLOB)
            {
              const NDBCOL *col= tab->getColumn(field->field_name);
              DBUG_ASSERT(col);
              curr_cond->ndb_item= new Ndb_item(field, col->getColumnNo());
              context->dont_expect(Item::FIELD_ITEM);
              context->expect_no_field_result();
              if (! context->expecting_nothing())
              {
                // We have not seen second argument yet
                if (type == MYSQL_TYPE_TIME ||
                    type == MYSQL_TYPE_DATE || 
                    type == MYSQL_TYPE_YEAR ||
                    type == MYSQL_TYPE_DATETIME)
                {
                  context->expect_only(Item::STRING_ITEM);
                  context->expect(Item::INT_ITEM);
                }
                else
                  switch (field->result_type()) {
                  case STRING_RESULT:
                    // Expect char string or binary string
                    context->expect_only(Item::STRING_ITEM);
                    context->expect(Item::VARBIN_ITEM);
                    context->expect_collation(field_item->collation.collation);
                    break;
                  case REAL_RESULT:
                    context->expect_only(Item::REAL_ITEM);
                    context->expect(Item::DECIMAL_ITEM);
                    context->expect(Item::INT_ITEM);
                    break;
                  case INT_RESULT:
                    context->expect_only(Item::INT_ITEM);
                    context->expect(Item::VARBIN_ITEM);
                    break;
                  case DECIMAL_RESULT:
                    context->expect_only(Item::DECIMAL_ITEM);
                    context->expect(Item::REAL_ITEM);
                    context->expect(Item::INT_ITEM);
                    break;
                  default:
                    break;
                  }    
              }
              else
              {
                // Expect another logical expression
                context->expect_only(Item::FUNC_ITEM);
                context->expect(Item::COND_ITEM);
                // Check that field and string constant collations are the same
                if ((field->result_type() == STRING_RESULT) &&
                    !context->expecting_collation(item->collation.collation)
                    && type != MYSQL_TYPE_TIME
                    && type != MYSQL_TYPE_DATE
                    && type != MYSQL_TYPE_YEAR
                    && type != MYSQL_TYPE_DATETIME)
                {
                  DBUG_PRINT("info", ("Found non-matching collation %s",  
                                      item->collation.collation->name)); 
                  context->supported= FALSE;                
                }
              }
              break;
            }
            else
            {
              DBUG_PRINT("info", ("Was not expecting field of type %u(%u)",
                                  field->result_type(), type));
              context->supported= FALSE;
            }
          }
          else
          {
            DBUG_PRINT("info", ("Was not expecting field from table %s (%s)",
                                context->table->s->table_name.str, 
                                field->table->s->table_name.str));
            context->supported= FALSE;
          }
          break;
        }
        case Item::FUNC_ITEM:
        {
          Item_func *func_item= (Item_func *) item;
          // Check that we expect a function or functional expression here
          if (context->expecting(Item::FUNC_ITEM) ||
              func_item->functype() == Item_func::UNKNOWN_FUNC ||
              func_item->functype() == Item_func::NEG_FUNC)
            context->expect_nothing();
          else
          {
            // Did not expect function here
            context->supported= FALSE;
            break;
          }
          
          switch (func_item->functype()) {
          case Item_func::EQ_FUNC:
          {
            DBUG_PRINT("info", ("EQ_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(), 
                                              func_item);      
            context->expect(Item::STRING_ITEM);
            context->expect(Item::INT_ITEM);
            context->expect(Item::REAL_ITEM);
            context->expect(Item::DECIMAL_ITEM);
            context->expect(Item::VARBIN_ITEM);
            context->expect(Item::FIELD_ITEM);
            context->expect_field_result(STRING_RESULT);
            context->expect_field_result(REAL_RESULT);
            context->expect_field_result(INT_RESULT);
            context->expect_field_result(DECIMAL_RESULT);
            break;
          }
          case Item_func::NE_FUNC:
          {
            DBUG_PRINT("info", ("NE_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(),
                                              func_item);      
            context->expect(Item::STRING_ITEM);
            context->expect(Item::INT_ITEM);
            context->expect(Item::REAL_ITEM);
            context->expect(Item::DECIMAL_ITEM);
            context->expect(Item::VARBIN_ITEM);
            context->expect(Item::FIELD_ITEM);
            context->expect_field_result(STRING_RESULT);
            context->expect_field_result(REAL_RESULT);
            context->expect_field_result(INT_RESULT);
            context->expect_field_result(DECIMAL_RESULT);
            break;
          }
          case Item_func::LT_FUNC:
          {
            DBUG_PRINT("info", ("LT_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(),
                                              func_item);      
            context->expect(Item::STRING_ITEM);
            context->expect(Item::INT_ITEM);
            context->expect(Item::REAL_ITEM);
            context->expect(Item::DECIMAL_ITEM);
            context->expect(Item::VARBIN_ITEM);
            context->expect(Item::FIELD_ITEM);
            context->expect_field_result(STRING_RESULT);
            context->expect_field_result(REAL_RESULT);
            context->expect_field_result(INT_RESULT);
            context->expect_field_result(DECIMAL_RESULT);
            break;
          }
          case Item_func::LE_FUNC:
          {
            DBUG_PRINT("info", ("LE_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(),
                                              func_item);      
            context->expect(Item::STRING_ITEM);
            context->expect(Item::INT_ITEM);
            context->expect(Item::REAL_ITEM);
            context->expect(Item::DECIMAL_ITEM);
            context->expect(Item::VARBIN_ITEM);
            context->expect(Item::FIELD_ITEM);
            context->expect_field_result(STRING_RESULT);
            context->expect_field_result(REAL_RESULT);
            context->expect_field_result(INT_RESULT);
            context->expect_field_result(DECIMAL_RESULT);
            break;
          }
          case Item_func::GE_FUNC:
          {
            DBUG_PRINT("info", ("GE_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(),
                                              func_item);      
            context->expect(Item::STRING_ITEM);
            context->expect(Item::INT_ITEM);
            context->expect(Item::REAL_ITEM);
            context->expect(Item::DECIMAL_ITEM);
            context->expect(Item::VARBIN_ITEM);
            context->expect(Item::FIELD_ITEM);
            context->expect_field_result(STRING_RESULT);
            context->expect_field_result(REAL_RESULT);
            context->expect_field_result(INT_RESULT);
            context->expect_field_result(DECIMAL_RESULT);
            break;
          }
          case Item_func::GT_FUNC:
          {
            DBUG_PRINT("info", ("GT_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(),
                                              func_item);      
            context->expect(Item::STRING_ITEM);
            context->expect(Item::REAL_ITEM);
            context->expect(Item::DECIMAL_ITEM);
            context->expect(Item::INT_ITEM);
            context->expect(Item::VARBIN_ITEM);
            context->expect(Item::FIELD_ITEM);
            context->expect_field_result(STRING_RESULT);
            context->expect_field_result(REAL_RESULT);
            context->expect_field_result(INT_RESULT);
            context->expect_field_result(DECIMAL_RESULT);
            break;
          }
          case Item_func::LIKE_FUNC:
          {
            DBUG_PRINT("info", ("LIKE_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(),
                                              func_item);      
            context->expect(Item::STRING_ITEM);
            context->expect(Item::FIELD_ITEM);
            context->expect_only_field_type(MYSQL_TYPE_STRING);
            context->expect_field_type(MYSQL_TYPE_VAR_STRING);
            context->expect_field_type(MYSQL_TYPE_VARCHAR);
            context->expect_field_result(STRING_RESULT);
            context->expect(Item::FUNC_ITEM);
            break;
          }
          case Item_func::ISNULL_FUNC:
          {
            DBUG_PRINT("info", ("ISNULL_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(),
                                              func_item);      
            context->expect(Item::FIELD_ITEM);
            context->expect_field_result(STRING_RESULT);
            context->expect_field_result(REAL_RESULT);
            context->expect_field_result(INT_RESULT);
            context->expect_field_result(DECIMAL_RESULT);
            break;
          }
          case Item_func::ISNOTNULL_FUNC:
          {
            DBUG_PRINT("info", ("ISNOTNULL_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(),
                                              func_item);     
            context->expect(Item::FIELD_ITEM);
            context->expect_field_result(STRING_RESULT);
            context->expect_field_result(REAL_RESULT);
            context->expect_field_result(INT_RESULT);
            context->expect_field_result(DECIMAL_RESULT);
            break;
          }
          case Item_func::NOT_FUNC:
          {
            DBUG_PRINT("info", ("NOT_FUNC"));      
            curr_cond->ndb_item= new Ndb_item(func_item->functype(),
                                              func_item);     
            context->expect(Item::FUNC_ITEM);
            context->expect(Item::COND_ITEM);
            break;
          }
          case Item_func::BETWEEN:
          {
            DBUG_PRINT("info", ("BETWEEN, rewriting using AND"));
            Item_func_between *between_func= (Item_func_between *) func_item;
            Ndb_rewrite_context *rewrite_context= 
              new Ndb_rewrite_context(func_item);
            rewrite_context->next= context->rewrite_stack;
            context->rewrite_stack= rewrite_context;
            if (between_func->negated)
            {
              DBUG_PRINT("info", ("NOT_FUNC"));
              curr_cond->ndb_item= new Ndb_item(Item_func::NOT_FUNC, 1);
              prev_cond= curr_cond;
              curr_cond= context->cond_ptr= new Ndb_cond();
              curr_cond->prev= prev_cond;
              prev_cond->next= curr_cond;
            }
            DBUG_PRINT("info", ("COND_AND_FUNC"));
            curr_cond->ndb_item= 
              new Ndb_item(Item_func::COND_AND_FUNC, 
                           func_item->argument_count() - 1);
            context->expect_only(Item::FIELD_ITEM);
            context->expect(Item::INT_ITEM);
            context->expect(Item::STRING_ITEM);
            context->expect(Item::VARBIN_ITEM);
            context->expect(Item::FUNC_ITEM);
            break;
          }
          case Item_func::IN_FUNC:
          {
            DBUG_PRINT("info", ("IN_FUNC, rewriting using OR"));
            Item_func_in *in_func= (Item_func_in *) func_item;
            Ndb_rewrite_context *rewrite_context= 
              new Ndb_rewrite_context(func_item);
            rewrite_context->next= context->rewrite_stack;
            context->rewrite_stack= rewrite_context;
            if (in_func->negated)
            {
              DBUG_PRINT("info", ("NOT_FUNC"));
              curr_cond->ndb_item= new Ndb_item(Item_func::NOT_FUNC, 1);
              prev_cond= curr_cond;
              curr_cond= context->cond_ptr= new Ndb_cond();
              curr_cond->prev= prev_cond;
              prev_cond->next= curr_cond;
            }
            DBUG_PRINT("info", ("COND_OR_FUNC"));
            curr_cond->ndb_item= new Ndb_item(Item_func::COND_OR_FUNC, 
                                              func_item->argument_count() - 1);
            context->expect_only(Item::FIELD_ITEM);
            context->expect(Item::INT_ITEM);
            context->expect(Item::STRING_ITEM);
            context->expect(Item::VARBIN_ITEM);
            context->expect(Item::FUNC_ITEM);
            break;
          }
          case Item_func::NEG_FUNC:
          case Item_func::UNKNOWN_FUNC:
          {
            DBUG_PRINT("info", ("UNKNOWN_FUNC %s", 
                                func_item->const_item()?"const":""));  
            DBUG_PRINT("info", ("result type %d", func_item->result_type()));
            if (func_item->const_item())
            {
              ndb_serialize_const(func_item->result_type(), item, curr_cond,
                                  context);

              // Skip any arguments since we will evaluate function instead
              DBUG_PRINT("info", ("Skip until end of arguments marker"));
              context->skip= func_item->argument_count();
            }
            else
              // Function does not return constant expression
              context->supported= FALSE;
            break;
          }
          default:
          {
            DBUG_PRINT("info", ("Found func_item of type %d", 
                                func_item->functype()));
            context->supported= FALSE;
          }
          }
          break;
        }
        case Item::STRING_ITEM:
          DBUG_PRINT("info", ("STRING_ITEM")); 
          if (context->expecting(Item::STRING_ITEM)) 
          {
#ifndef DBUG_OFF
            char buff[256];
            String str(buff,(uint32) sizeof(buff), system_charset_info);
            str.length(0);
            Item_string *string_item= (Item_string *) item;
            DBUG_PRINT("info", ("value \"%s\"", 
                                string_item->val_str(&str)->ptr()));
#endif
            NDB_ITEM_QUALIFICATION q;
            q.value_type= Item::STRING_ITEM;
            curr_cond->ndb_item= new Ndb_item(NDB_VALUE, q, item);      
            if (! context->expecting_no_field_result())
            {
              // We have not seen the field argument yet
              context->expect_only(Item::FIELD_ITEM);
              context->expect_only_field_result(STRING_RESULT);
              context->expect_collation(item->collation.collation);
            }
            else 
            {
              // Expect another logical expression
              context->expect_only(Item::FUNC_ITEM);
              context->expect(Item::COND_ITEM);
              // Check that we are comparing with a field with same collation
              if (!context->expecting_collation(item->collation.collation))
              {
                DBUG_PRINT("info", ("Found non-matching collation %s",  
                                    item->collation.collation->name));
                context->supported= FALSE;
              }
            }
          }
          else
            context->supported= FALSE;
          break;
        case Item::INT_ITEM:
          DBUG_PRINT("info", ("INT_ITEM"));
          if (context->expecting(Item::INT_ITEM)) 
          {
            DBUG_PRINT("info", ("value %ld",
                                (long) ((Item_int*) item)->value));
            NDB_ITEM_QUALIFICATION q;
            q.value_type= Item::INT_ITEM;
            curr_cond->ndb_item= new Ndb_item(NDB_VALUE, q, item);
            if (! context->expecting_no_field_result()) 
            {
              // We have not seen the field argument yet
              context->expect_only(Item::FIELD_ITEM);
              context->expect_only_field_result(INT_RESULT);
              context->expect_field_result(REAL_RESULT);
              context->expect_field_result(DECIMAL_RESULT);
            }
            else
            {
              // Expect another logical expression
              context->expect_only(Item::FUNC_ITEM);
              context->expect(Item::COND_ITEM);
            }
          }
          else
            context->supported= FALSE;
          break;
        case Item::REAL_ITEM:
          DBUG_PRINT("info", ("REAL_ITEM"));
          if (context->expecting(Item::REAL_ITEM)) 
          {
            DBUG_PRINT("info", ("value %f", ((Item_float*) item)->value));
            NDB_ITEM_QUALIFICATION q;
            q.value_type= Item::REAL_ITEM;
            curr_cond->ndb_item= new Ndb_item(NDB_VALUE, q, item);
            if (! context->expecting_no_field_result()) 
            {
              // We have not seen the field argument yet
              context->expect_only(Item::FIELD_ITEM);
              context->expect_only_field_result(REAL_RESULT);
            }
            else
            {
              // Expect another logical expression
              context->expect_only(Item::FUNC_ITEM);
              context->expect(Item::COND_ITEM);
            }
          }
          else
            context->supported= FALSE;
          break;
        case Item::VARBIN_ITEM:
          DBUG_PRINT("info", ("VARBIN_ITEM"));
          if (context->expecting(Item::VARBIN_ITEM)) 
          {
            NDB_ITEM_QUALIFICATION q;
            q.value_type= Item::VARBIN_ITEM;
            curr_cond->ndb_item= new Ndb_item(NDB_VALUE, q, item);      
            if (! context->expecting_no_field_result())
            {
              // We have not seen the field argument yet
              context->expect_only(Item::FIELD_ITEM);
              context->expect_only_field_result(STRING_RESULT);
            }
            else
            {
              // Expect another logical expression
              context->expect_only(Item::FUNC_ITEM);
              context->expect(Item::COND_ITEM);
            }
          }
          else
            context->supported= FALSE;
          break;
        case Item::DECIMAL_ITEM:
          DBUG_PRINT("info", ("DECIMAL_ITEM"));
          if (context->expecting(Item::DECIMAL_ITEM)) 
          {
            DBUG_PRINT("info", ("value %f",
                                ((Item_decimal*) item)->val_real()));
            NDB_ITEM_QUALIFICATION q;
            q.value_type= Item::DECIMAL_ITEM;
            curr_cond->ndb_item= new Ndb_item(NDB_VALUE, q, item);
            if (! context->expecting_no_field_result()) 
            {
              // We have not seen the field argument yet
              context->expect_only(Item::FIELD_ITEM);
              context->expect_only_field_result(REAL_RESULT);
              context->expect_field_result(DECIMAL_RESULT);
            }
            else
            {
              // Expect another logical expression
              context->expect_only(Item::FUNC_ITEM);
              context->expect(Item::COND_ITEM);
            }
          }
          else
            context->supported= FALSE;
          break;
        case Item::COND_ITEM:
        {
          Item_cond *cond_item= (Item_cond *) item;
          
          if (context->expecting(Item::COND_ITEM))
          {
            switch (cond_item->functype()) {
            case Item_func::COND_AND_FUNC:
              DBUG_PRINT("info", ("COND_AND_FUNC"));
              curr_cond->ndb_item= new Ndb_item(cond_item->functype(),
                                                cond_item);      
              break;
            case Item_func::COND_OR_FUNC:
              DBUG_PRINT("info", ("COND_OR_FUNC"));
              curr_cond->ndb_item= new Ndb_item(cond_item->functype(),
                                                cond_item);      
              break;
            default:
              DBUG_PRINT("info", ("COND_ITEM %d", cond_item->functype()));
              context->supported= FALSE;
              break;
            }
          }
          else
          {
            /* Did not expect condition */
            context->supported= FALSE;          
          }
          break;
        }
        case Item::CACHE_ITEM:
        {
          DBUG_PRINT("info", ("CACHE_ITEM"));
          if (item->const_item())
          {
            ndb_serialize_const(((Item_cache*)item)->result_type(), item,
                                curr_cond, context);
          }
          else
            context->supported= FALSE;

          break;
        }
        default:
        {
          DBUG_PRINT("info", ("Found item of type %d", item->type()));
          context->supported= FALSE;
        }
        }
      }
      if (context->supported && context->rewrite_stack)
      {
        Ndb_rewrite_context *rewrite_context= context->rewrite_stack;
        if (rewrite_context->count == 
            rewrite_context->func_item->argument_count())
        {
          // Rewrite is done, wrap an END() at the en
          DBUG_PRINT("info", ("End of condition group"));
          prev_cond= curr_cond;
          curr_cond= context->cond_ptr= new Ndb_cond();
          curr_cond->prev= prev_cond;
          prev_cond->next= curr_cond;
          curr_cond->ndb_item= new Ndb_item(NDB_END_COND);
          // Pop rewrite stack
          context->rewrite_stack=  rewrite_context->next;
          rewrite_context->next= NULL;
          delete(rewrite_context);
        }
      }
    }
  }
 
  DBUG_VOID_RETURN;
}

/*
  Push a condition
 */
const 
COND* 
ha_ndbcluster_cond::cond_push(const COND *cond, 
                              TABLE *table, const NDBTAB *ndb_table)
{ 
  DBUG_ENTER("cond_push");
  Ndb_cond_stack *ndb_cond = new Ndb_cond_stack();
  if (ndb_cond == NULL)
  {
    my_errno= HA_ERR_OUT_OF_MEM;
    DBUG_RETURN(NULL);
  }
  if (m_cond_stack)
    ndb_cond->next= m_cond_stack;
  else
    ndb_cond->next= NULL;
  m_cond_stack= ndb_cond;
  
  if (serialize_cond(cond, ndb_cond, table, ndb_table))
  {
    DBUG_RETURN(NULL);
  }
  else
  {
    cond_pop();
  }
  DBUG_RETURN(cond); 
}

/*
  Pop the top condition from the condition stack
*/
void 
ha_ndbcluster_cond::cond_pop() 
{ 
  Ndb_cond_stack *ndb_cond_stack= m_cond_stack;  
  if (ndb_cond_stack)
  {
    m_cond_stack= ndb_cond_stack->next;
    ndb_cond_stack->next= NULL;
    delete ndb_cond_stack;
  }
}

/*
  Clear the condition stack
*/
void
ha_ndbcluster_cond::cond_clear()
{
  DBUG_ENTER("cond_clear");
  while (m_cond_stack)
    cond_pop();

  DBUG_VOID_RETURN;
}

bool
ha_ndbcluster_cond::serialize_cond(const COND *cond, Ndb_cond_stack *ndb_cond,
                                   TABLE *table, const NDBTAB *ndb_table)
{
  DBUG_ENTER("serialize_cond");
  Item *item= (Item *) cond;
  Ndb_cond_traverse_context context(table, ndb_table, ndb_cond);
  // Expect a logical expression
  context.expect(Item::FUNC_ITEM);
  context.expect(Item::COND_ITEM);
  item->traverse_cond(&ndb_serialize_cond, (void *) &context, Item::PREFIX);
  DBUG_PRINT("info", ("The pushed condition is %ssupported", (context.supported)?"":"not "));

  DBUG_RETURN(context.supported);
}

int
ha_ndbcluster_cond::build_scan_filter_predicate(Ndb_cond * &cond, 
                                                NdbScanFilter *filter,
                                                bool negated)
{
  DBUG_ENTER("build_scan_filter_predicate");  
  switch (cond->ndb_item->type) {
  case NDB_FUNCTION:
  {
    if (!cond->next)
      break;
    Ndb_item *a= cond->next->ndb_item;
    Ndb_item *b, *field, *value= NULL;

    switch (cond->ndb_item->argument_count()) {
    case 1:
      field= (a->type == NDB_FIELD)? a : NULL;
      break;
    case 2:
      if (!cond->next->next)
      {
        field= NULL;
        break;
      }
      b= cond->next->next->ndb_item;
      value= ((a->type == NDB_VALUE) ? a :
              (b->type == NDB_VALUE) ? b :
              NULL);
      field= ((a->type == NDB_FIELD) ? a :
              (b->type == NDB_FIELD) ? b :
              NULL);
      break;
    default:
      field= NULL; //Keep compiler happy
      DBUG_ASSERT(0);
      break;
    }
    switch ((negated) ? 
            Ndb_item::negate(cond->ndb_item->qualification.function_type)
            : cond->ndb_item->qualification.function_type) {
    case NDB_EQ_FUNC:
    {
      if (!value || !field) break;
      // Save value in right format for the field type
      value->save_in_field(field);
      DBUG_PRINT("info", ("Generating EQ filter"));
      if (filter->cmp(NdbScanFilter::COND_EQ, 
                      field->get_field_no(),
                      field->get_val(),
                      field->pack_length()) == -1)
        DBUG_RETURN(1);
      cond= cond->next->next->next;
      DBUG_RETURN(0);
    }
    case NDB_NE_FUNC:
    {
      if (!value || !field) break;
      // Save value in right format for the field type
      value->save_in_field(field);
      DBUG_PRINT("info", ("Generating NE filter"));
      if (filter->cmp(NdbScanFilter::COND_NE, 
                      field->get_field_no(),
                      field->get_val(),
                      field->pack_length()) == -1)
        DBUG_RETURN(1);
      cond= cond->next->next->next;
      DBUG_RETURN(0);
    }
    case NDB_LT_FUNC:
    {
      if (!value || !field) break;
      // Save value in right format for the field type
      value->save_in_field(field);
      if (a == field)
      {
        DBUG_PRINT("info", ("Generating LT filter")); 
        if (filter->cmp(NdbScanFilter::COND_LT, 
                        field->get_field_no(),
                        field->get_val(),
                        field->pack_length()) == -1)
          DBUG_RETURN(1);
      }
      else
      {
        DBUG_PRINT("info", ("Generating GT filter")); 
        if (filter->cmp(NdbScanFilter::COND_GT, 
                        field->get_field_no(),
                        field->get_val(),
                        field->pack_length()) == -1)
          DBUG_RETURN(1);
      }
      cond= cond->next->next->next;
      DBUG_RETURN(0);
    }
    case NDB_LE_FUNC:
    {
      if (!value || !field) break;
      // Save value in right format for the field type
      value->save_in_field(field);
      if (a == field)
      {
        DBUG_PRINT("info", ("Generating LE filter")); 
        if (filter->cmp(NdbScanFilter::COND_LE, 
                        field->get_field_no(),
                        field->get_val(),
                        field->pack_length()) == -1)
          DBUG_RETURN(1);       
      }
      else
      {
        DBUG_PRINT("info", ("Generating GE filter")); 
        if (filter->cmp(NdbScanFilter::COND_GE, 
                        field->get_field_no(),
                        field->get_val(),
                        field->pack_length()) == -1)
          DBUG_RETURN(1);
      }
      cond= cond->next->next->next;
      DBUG_RETURN(0);
    }
    case NDB_GE_FUNC:
    {
      if (!value || !field) break;
      // Save value in right format for the field type
      value->save_in_field(field);
      if (a == field)
      {
        DBUG_PRINT("info", ("Generating GE filter")); 
        if (filter->cmp(NdbScanFilter::COND_GE, 
                        field->get_field_no(),
                        field->get_val(),
                        field->pack_length()) == -1)
          DBUG_RETURN(1);
      }
      else
      {
        DBUG_PRINT("info", ("Generating LE filter")); 
        if (filter->cmp(NdbScanFilter::COND_LE, 
                        field->get_field_no(),
                        field->get_val(),
                        field->pack_length()) == -1)
          DBUG_RETURN(1);
      }
      cond= cond->next->next->next;
      DBUG_RETURN(0);
    }
    case NDB_GT_FUNC:
    {
      if (!value || !field) break;
      // Save value in right format for the field type
      value->save_in_field(field);
      if (a == field)
      {
        DBUG_PRINT("info", ("Generating GT filter"));
        if (filter->cmp(NdbScanFilter::COND_GT, 
                        field->get_field_no(),
                        field->get_val(),
                        field->pack_length()) == -1)
          DBUG_RETURN(1);
      }
      else
      {
        DBUG_PRINT("info", ("Generating LT filter"));
        if (filter->cmp(NdbScanFilter::COND_LT, 
                        field->get_field_no(),
                        field->get_val(),
                        field->pack_length()) == -1)
          DBUG_RETURN(1);
      }
      cond= cond->next->next->next;
      DBUG_RETURN(0);
    }
    case NDB_LIKE_FUNC:
    {
      if (!value || !field) break;
      if ((value->qualification.value_type != Item::STRING_ITEM) &&
          (value->qualification.value_type != Item::VARBIN_ITEM))
          break;
      // Save value in right format for the field type
      value->save_in_field(field);
      DBUG_PRINT("info", ("Generating LIKE filter: like(%d,%s,%d)", 
                          field->get_field_no(), value->get_val(), 
                          value->pack_length()));
      if (filter->cmp(NdbScanFilter::COND_LIKE, 
                      field->get_field_no(),
                      value->get_val(),
                      value->pack_length()) == -1)
        DBUG_RETURN(1);
      cond= cond->next->next->next;
      DBUG_RETURN(0);
    }
    case NDB_NOTLIKE_FUNC:
    {
      if (!value || !field) break;
      if ((value->qualification.value_type != Item::STRING_ITEM) &&
          (value->qualification.value_type != Item::VARBIN_ITEM))
          break;
      // Save value in right format for the field type
      value->save_in_field(field);
      DBUG_PRINT("info", ("Generating NOTLIKE filter: notlike(%d,%s,%d)", 
                          field->get_field_no(), value->get_val(), 
                          value->pack_length()));
      if (filter->cmp(NdbScanFilter::COND_NOT_LIKE, 
                      field->get_field_no(),
                      value->get_val(),
                      value->pack_length()) == -1)
        DBUG_RETURN(1);
      cond= cond->next->next->next;
      DBUG_RETURN(0);
    }
    case NDB_ISNULL_FUNC:
      if (!field)
        break;
      DBUG_PRINT("info", ("Generating ISNULL filter"));
      if (filter->isnull(field->get_field_no()) == -1)
        DBUG_RETURN(1);
      cond= cond->next->next;
      DBUG_RETURN(0);
    case NDB_ISNOTNULL_FUNC:
    {
      if (!field)
        break;
      DBUG_PRINT("info", ("Generating ISNOTNULL filter"));
      if (filter->isnotnull(field->get_field_no()) == -1)
        DBUG_RETURN(1);         
      cond= cond->next->next;
      DBUG_RETURN(0);
    }
    default:
      break;
    }
    break;
  }
  default:
    break;
  }
  DBUG_PRINT("info", ("Found illegal condition"));
  DBUG_RETURN(1);
}


int
ha_ndbcluster_cond::build_scan_filter_group(Ndb_cond* &cond, 
                                            NdbScanFilter *filter)
{
  uint level=0;
  bool negated= FALSE;
  DBUG_ENTER("build_scan_filter_group");

  do
  {
    if (!cond)
      DBUG_RETURN(1);
    switch (cond->ndb_item->type) {
    case NDB_FUNCTION:
    {
      switch (cond->ndb_item->qualification.function_type) {
      case NDB_COND_AND_FUNC:
      {
        level++;
        DBUG_PRINT("info", ("Generating %s group %u", (negated)?"NAND":"AND",
                            level));
        if ((negated) ? filter->begin(NdbScanFilter::NAND)
            : filter->begin(NdbScanFilter::AND) == -1)
          DBUG_RETURN(1);
        negated= FALSE;
        cond= cond->next;
        break;
      }
      case NDB_COND_OR_FUNC:
      {
        level++;
        DBUG_PRINT("info", ("Generating %s group %u", (negated)?"NOR":"OR",
                            level));
        if ((negated) ? filter->begin(NdbScanFilter::NOR)
            : filter->begin(NdbScanFilter::OR) == -1)
          DBUG_RETURN(1);
        negated= FALSE;
        cond= cond->next;
        break;
      }
      case NDB_NOT_FUNC:
      {
        DBUG_PRINT("info", ("Generating negated query"));
        cond= cond->next;
        negated= TRUE;
        break;
      }
      default:
        if (build_scan_filter_predicate(cond, filter, negated))
          DBUG_RETURN(1);
        negated= FALSE;
        break;
      }
      break;
    }
    case NDB_END_COND:
      DBUG_PRINT("info", ("End of group %u", level));
      level--;
      if (cond) cond= cond->next;
      if (filter->end() == -1)
        DBUG_RETURN(1);
      if (!negated)
        break;
      // else fall through (NOT END is an illegal condition)
    default:
    {
      DBUG_PRINT("info", ("Illegal scan filter"));
    }
    }
  }  while (level > 0 || negated);
  
  DBUG_RETURN(0);
}


int
ha_ndbcluster_cond::build_scan_filter(Ndb_cond * &cond, NdbScanFilter *filter)
{
  bool simple_cond= TRUE;
  DBUG_ENTER("build_scan_filter");  

    switch (cond->ndb_item->type) {
    case NDB_FUNCTION:
      switch (cond->ndb_item->qualification.function_type) {
      case NDB_COND_AND_FUNC:
      case NDB_COND_OR_FUNC:
        simple_cond= FALSE;
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  if (simple_cond && filter->begin() == -1)
    DBUG_RETURN(1);
  if (build_scan_filter_group(cond, filter))
    DBUG_RETURN(1);
  if (simple_cond && filter->end() == -1)
    DBUG_RETURN(1);

  DBUG_RETURN(0);
}

int
ha_ndbcluster_cond::generate_scan_filter(NdbScanOperation *op)
{
  DBUG_ENTER("generate_scan_filter");

  if (m_cond_stack)
  {
    NdbScanFilter filter(op, false); // don't abort on too large
    
    int ret=generate_scan_filter_from_cond(filter);
    if (ret != 0)
    {
      const NdbError& err=filter.getNdbError();
      if (err.code == NdbScanFilter::FilterTooLarge)
      {
        // err.message has static storage
        DBUG_PRINT("info", ("%s", err.message));
        push_warning(current_thd, Sql_condition::WARN_LEVEL_WARN,
                     err.code, err.message);
        ret=0;
      }
    }
    if (ret != 0)
      DBUG_RETURN(ret);
  }
  else
  {  
    DBUG_PRINT("info", ("Empty stack"));
  }

  DBUG_RETURN(0);
}


int
ha_ndbcluster_cond::generate_scan_filter_from_cond(NdbScanFilter& filter)
{
  bool multiple_cond= FALSE;
  DBUG_ENTER("generate_scan_filter_from_cond");

  // Wrap an AND group around multiple conditions
  if (m_cond_stack->next) 
  {
    multiple_cond= TRUE;
    if (filter.begin() == -1)
      DBUG_RETURN(1); 
  }
  for (Ndb_cond_stack *stack= m_cond_stack; 
       (stack); 
       stack= stack->next)
  {
    Ndb_cond *cond= stack->ndb_cond;
    
    if (build_scan_filter(cond, &filter))
    {
      DBUG_PRINT("info", ("build_scan_filter failed"));
      DBUG_RETURN(1);
    }
  }
  if (multiple_cond && filter.end() == -1)
    DBUG_RETURN(1);

  DBUG_RETURN(0);
}


int ha_ndbcluster_cond::generate_scan_filter_from_key(NdbScanOperation *op,
                                                      const KEY* key_info, 
                                                      const uchar *key, 
                                                      uint key_len,
                                                      uchar *buf)
{
  KEY_PART_INFO* key_part= key_info->key_part;
  KEY_PART_INFO* end= key_part+key_info->user_defined_key_parts;
  NdbScanFilter filter(op, true); // abort on too large
  int res;
  DBUG_ENTER("generate_scan_filter_from_key");

  filter.begin(NdbScanFilter::AND);
  for (; key_part != end; key_part++) 
  {
    Field* field= key_part->field;
    uint32 pack_len= field->pack_length();
    const uchar* ptr= key;
    DBUG_PRINT("info", ("Filtering value for %s", field->field_name));
    DBUG_DUMP("key", ptr, pack_len);
    if (key_part->null_bit)
    {
      DBUG_PRINT("info", ("Generating ISNULL filter"));
      if (filter.isnull(key_part->fieldnr-1) == -1)
	DBUG_RETURN(1);
    }
    else
    {
      DBUG_PRINT("info", ("Generating EQ filter"));
      if (filter.cmp(NdbScanFilter::COND_EQ, 
		     key_part->fieldnr-1,
		     ptr,
		     pack_len) == -1)
	DBUG_RETURN(1);
    }
    key += key_part->store_length;
  }      
  // Add any pushed condition
  if (m_cond_stack &&
      (res= generate_scan_filter_from_cond(filter)))
    DBUG_RETURN(res);
    
  if (filter.end() == -1)
    DBUG_RETURN(1);

  DBUG_RETURN(0);
}

#endif
