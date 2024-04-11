#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int Addition(int num1,int num2){
return num1+num2;
}
int Subtraction(int num1,int num2){
return num1-num2;
}
int Multiplication(int num1,int num2){
return num1*num2;
}
int Division(int num1,int num2){
return num1/num2;
}


int main()
{
   int firstnumber,secondnumber,operationnumber;
   printf("------------DEVELOPED SCIENTIFIC 4 FUNCTION CALCULATOR ---------------\n");
   printf("Functions used in my calculator\n");
   printf("1.Addition\n");
   printf("2.Subtraction\n");
   printf("3.Multiplication\n");
   printf("4.Division\n");

//   printf("The sine of 90 is %d",sine(90));

   printf("Enter the first number of your choice: \n");
   scanf("%d", &firstnumber);
   printf("Enter the second number of your choice: \n");
   scanf("%d", &secondnumber);
   printf("Enter the number in front of the operations given above: \n");
   scanf("%d", &operationnumber);



    if(operationnumber==1){
        printf("The addition of %d and %d gives %d", firstnumber, secondnumber, Addition(firstnumber,secondnumber));

    }
    else if(operationnumber==2){
        printf("The subtraction of %d and %d gives %d", firstnumber, secondnumber, Subtraction(firstnumber,secondnumber));
    }
    else if(operationnumber==3){
        printf("The multiplication of %d and %d gives %d", firstnumber, secondnumber, Multiplication(firstnumber,secondnumber));

    }
    else if(operationnumber==4){
        printf("The division of %d and %d gives %d",firstnumber,secondnumber,Division(firstnumber,secondnumber));
    }

    else
        printf("The division of %d and %d is %d", firstnumber, secondnumber, Division(firstnumber,secondnumber));

  return 0;
}

