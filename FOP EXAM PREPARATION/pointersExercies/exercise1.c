#include <stdio.h>

int main() {
    // Declare and initialize variables
    double myDouble = 3.14;
    int myInt = 42;
    char myChar = 'A';

    // Declare and initialize pointers
    double *doublePtr = &myDouble;
    int *intPtr = &myInt;
    char *charPtr = &myChar;

    // Print addresses and values
    printf("The address of char___ is 0x%x\n", (unsigned int)&myChar);
    printf("The address of int___ is 0x%x\n", (unsigned int)&myInt);
    printf("The address of double ___ is 0x%x\n", (unsigned int)&myDouble);
    printf("The address of char*___ is 0x%x\n", (unsigned int)&charPtr);
    printf("The address of int*___ is 0x%x\n", (unsigned int)&intPtr);
    printf("The address of double* ___ is 0x%x\n", (unsigned int)&doublePtr);

    printf("The value of char___ is %c\n", myChar);
    printf("The value of int___ is %d\n", myInt);
    printf("The value of double ___ is %f\n", myDouble);
    printf("The value of char*___ is %p\n", (void *)charPtr);
    printf("The value of int*___ is %p\n", (void *)intPtr);
    printf("The value of double* ___ is %p\n", (void *)doublePtr);

    // Print sizes
    printf("The size of char___ is %lu bytes\n", sizeof(myChar));
    printf("The size of int___ is %lu bytes\n", sizeof(myInt));
    printf("The size of double ___ is %lu bytes\n", sizeof(myDouble));
    printf("The size of char*___ is %lu bytes\n", sizeof(charPtr));
    printf("The size of int*___ is %lu bytes\n", sizeof(intPtr));
    printf("The size of double* ___ is %lu bytes\n", sizeof(doublePtr));

    return 0;
}
