#include <stdlib.h>
#include <stdio.h>


int fibonacci(int a) {
    if (a <= 0) {
        return 0;
    } else if (a == 1) {
        return 1;
    } else {
        return fibonacci(a - 1) + fibonacci(a - 2);
    }
}


void printFibonacciSeries(int a) {
    if (a <= 0) {
        printf("Please enter a positive integer.\n");
        return;
    }

    for (int i = 0; i < a; i++) {
        printf("%d ", fibonacci(i));  
    }
    printf("\n");  
}

int main() {
    int a;

    printf("Enter the number of terms: \n");
    scanf("%d", &a);

    printFibonacciSeries(a);

    return 0;
}
