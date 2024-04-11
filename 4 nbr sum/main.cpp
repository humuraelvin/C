#include<stdio.h>
#include<stdlib.h>


int main(){

    double num1, num2, num3, num4, sum;

    printf("FOUR NUMBER SUMING\n");
    printf("enter the first number");
    scanf("%lf", &num1);
    printf("enter the second number ");
    scanf("%lf", &num2);
    printf("enter the third number");
    scanf("%lf",&num3);
    printf("enter the fourth number");
    scanf("%lf", &num4);

    sum=num1+num2+num3+num4;

    printf("the sum of the four numbers is= %lf", sum);





 return 0;
}
