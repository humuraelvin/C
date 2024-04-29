#include <stdio.h>

enum monthsOfTheYear {January, February, March, April, May, June, July, August, September, October, November, December};

int main(){

    int num = 0;

    printf("---MONTHS PROGRAM---\n");

    int i = 0;

    while (i==0)
    {
          printf("Enter a number from 0 up to 11\n");
    scanf("%d", &num);

    if (num < 0 || num > 11) {
        printf("Invalid number entered\n");
        return 1;
    }

    enum monthsOfTheYear thisMonth = num;

    switch (thisMonth)
    {
    case January:
        printf("We are in January");
        printf("\n");
        break;

    case February:
        printf("We are in February");
        printf("\n");
        break;

    case March:
        printf("We are in March");
        printf("\n");
        break;

    case April:
        printf("We are in April");
        printf("\n");
        break;
    
    case May:
        printf("We are in May");
        printf("\n");
        break;

    case June:
        printf("We are in June");
        printf("\n");
        break;

    case July:
        printf("We are in July");
        printf("\n");
        break;

    case August:
        printf("We are in August");
        printf("\n");
        break;

    case September:
        printf("We are in September");
        printf("\n");
        break;

    case October:
        printf("We are in October");
        printf("\n");
        break;
    
    case November:
        printf("We are in November");
        printf("\n");
        break;

    case December:
        printf("We are in December");
        printf("\n");
        break;

    default:
        printf("Invalid number entered");
        printf("\n");
        break;
    }
    }
    

 

    return 0;

}