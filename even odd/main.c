#include <stdio.h>
#include <stdlib.h>

int main()
{
    int number, even, odd;
    printf("ODD AND EVEN\n");
    printf("Enter a number: ", number);
    scanf("%d", number);
    if(number % 2==0){
        printf("THE number you have entered is an EVEN number", number);
    }
    else if(number % 2==1){
        printf("THE number you have entered is an ODD number", number);
    }

  return 0;
}
