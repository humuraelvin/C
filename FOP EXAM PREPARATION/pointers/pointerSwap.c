#include <stdlib.h>
#include <stdio.h>

int main(){

    int number1 = 50;
    int number2 = 100;

    int *pointer1 = &number1;
    int *pointer2 = &number2;

    //Before swap

    printf("Before swap:\n >*pointer1 = %d \n >*pointer2 = %d \n", *pointer1, *pointer2);

    //During swap

    *pointer1 = *pointer1 + *pointer2;
    *pointer2 = *pointer1 - *pointer2;
    *pointer1 = *pointer1 - *pointer2;
    
    //After swap

    printf("After swap:\n >*pointer1 = %d \n >*pointer2 = %d \n", *pointer1, *pointer2);


}
