//C Pass Addresses and Pointers
//In C programming, it is also possible to pass addresses as arguments to functions.

//To accept these addresses in the function definition, we can use pointers.
// It's because pointers are used to store addresses. Let's take an example:

//Example: Pass Addresses to Functions
#include <stdio.h>
void swap(int *n1, int *n2);

int main()
{
    int num1 = 5, num2 = 10;

    // address of num1 and num2 is passed
    swap( &num1, &num2);

    printf("num1 = %d\n", num1);
    printf("num2 = %d", num2);
    return 0;
}

void swap(int* n1, int* n2)
{
    int temp;
    temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

//When you run the program, the output will be:


//The address of num1 and num2 are passed to the swap() function using swap(&num1, &num2);.

//Pointers n1 and n2 accept these arguments in the function definition.

// void swap(int* n1, int* n2) {
//     ... ..
// }

// When *n1 and *n2 are changed inside the swap() function, num1 and num2 inside the main() function are also changed.

// Inside the swap() function, *n1 and *n2 swapped. Hence, num1 and num2 are also swapped.

// Notice that swap() is not returning anything; its return type is void.

// Example 2: Passing Pointers to Functions

#include <stdio.h>

void addOne(int* ptr) {
  (*ptr)++; // adding 1 to *ptr
}

// int main()
// {
//   int* p, i = 10;
//   p = &i;
//   addOne(p);

//   printf("%d", *p); // 11
//   return 0;
// }

// Here, the value stored at p, *p, is 10 initially.

// We then passed the pointer p to the addOne() function. The ptr pointer gets this address in the addOne() function.

// Inside the function, we increased the value stored at ptr by 1 using (*ptr)++;.
//  Since ptr and p pointers both have the same address, *p inside main() is also 11.