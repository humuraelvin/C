#include <stdio.h>
#include <stdlib.h>

int main()
{
    double num1, num2, num3;

    printf("Enter the first number\n");
    scanf("%lf", &num1);
    printf("Enter the second number\n");
    scanf("%lf", &num2);
    printf("Enter the third number\n");
    scanf("%lf", &num3);

    if(num1 > num2 && num1 > num3){
        printf("the largest is lf", num1);
    }
    else if(num2 > num1 && num2 > num3){
     printf("the largest is %lf", num2);
    }
    else if(num3 > num1 && num3 > num2){
     printf("the largest is %lf", num3);
    }
    return 0;
}
