#include <stdio.h>
#include <stdlib.h>

int main()
{
    int firstnumber,secondnumber,thirdnumber,sum;
    printf("*******THE CASIO CALCULATOR SINGLE  FUNCTION***********\n");
    printf("Enter the first number of your choice:\n");
    scanf("%d", &firstnumber);
    printf("Enter the second number of your choice:\n");
    scanf("%d", &secondnumber);
    printf("Enter the third number of your choice:\n");
    scanf("%d", &thirdnumber);

    sum=firstnumber+secondnumber+thirdnumber;

    printf("the sum of %d and %d and %d is %d", firstnumber, secondnumber, thirdnumber, sum);


    return 0;
}
