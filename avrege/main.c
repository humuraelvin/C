#include <stdio.h>
#include <stdlib.h>


 int average(int num1,int num2,int num3,int num4,int num5,int num6){
 int average = (num1+num2+num3+num4+num5+num6)/6;
 return average;
}

int main()
{
    int num1, num2, num3, num4, num5, num6, average;


    printf("Enter the first number of your choice\n");
    scanf("%d", &num1);
    printf("Enter the second number of your choice\n");
    scanf("%d", &num2);
    printf("Enter the third number of your choice\n");
    scanf("%d", &num3);
    printf("Enter the fourth number of your choice\n");
    scanf("%d", &num4);
    printf("Enter the fifth number of your choice\n");
    scanf("%d", &num5);
    printf("Enter the sixth number of your choice\n");
    scanf("%d", &num6);

    average=(num1+num2+num3+num4+num5+num6)/6;
    printf("The average of the numbers you entered is %d ",average);


    return 0;
}
