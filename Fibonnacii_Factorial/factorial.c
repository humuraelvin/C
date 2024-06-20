#include <stdlib.h>
#include <stdio.h>


int factorial(int n){

    if (n == 0 || n == 1)
    {
        return 1;
    }else if (n < 0)
    {
        printf("Please enter a positive integer");
    }else
    {
        return n * factorial(n-1);
    }

}

int main(){

    int n;

    printf("Enter a number: \n");
    scanf("%d", &n);

    printf("Its factorial: %d", factorial(n));

    return 0;

}