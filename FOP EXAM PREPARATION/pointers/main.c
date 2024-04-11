#include <stdio.h>
#include <stdlib.h>

int main(){
    int number = 50;
    int* p = &number;

    printf("The adress of the number is : %x \n", p);
    
    printf("The value is = %d", *p);

    //pointer to an array

    int array[10];
    int *p[10] = &array;

    //pointer to a function

    void show(int);
    void(*p)(int) = &show;

    //pointers to structs

    struct st
    {
        int i;
        float f;
    }ref;

    struct st *p = &ref;
    
    //ADVANTAGES OF POINTERS

    //1. POINTERS IMPROVES THE PERFOMANCE AND REDUCES THE CODES. THE CAN BE USED TO RETRIEVING STRINGS, TRESS,etc. They are also used with arrays, functions and structs
    //2. POINTERS allow you to be able to acess any memory location in the computer's memory
    //3. One can retrive multiple values from a function using a pointer

    //USAGE OF POINTES
    //1. Dynamic memory allocation : One can create dynamic memory allocations using functions like malloc() and calloc() by pointers
    //2. Arrays, structures and pointers: Pointers are widely used with arrays, functions and pointers. They reduce code and improve perfomance

    //Adress of (&) operator

    int value = 50;
    printf("The value of is = %d and its adress is : %u", value, &value);

    //NULL POINTER
    int *p =NULL;


    //how to read this pointer
    int (*p)(int (*)[2], int (*)(void));

    // The above pointer will be read as p is a pointer to such a function that which accepts the first parameter as pointer 
    // a one-dimensional array of integers of size two and the second parameter as pointer to function which parameter
    // void and return type is integer
    

}