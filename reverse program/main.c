#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stringReversing(char* str){

   int  length =  strlen(str);
   int stringStartpoint;
   int stringEndpoint;
   char revVar;

  for(stringStartpoint = 0, stringEndpoint = length - 1; stringStartpoint < stringEndpoint; stringStartpoint++, stringEndpoint--) {
    revVar = str[stringStartpoint];
    str[stringStartpoint] = str[stringEndpoint];
    str[stringEndpoint] = revVar;
   }
 }

int main()
{   char string[100];
    printf("---STRING REVERSING PROGRAM---\n");

    printf("ENTER A WORD OR A SENTENCE: \n");
    scanf("%s", string);

    stringReversing(string);
    printf("REVERSE IS: %s ", string);

    return 0;
}
