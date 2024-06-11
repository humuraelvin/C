#include <stdlib.h>
#include <stdio.h>

int factorial(int n){
    if(n==0 || n==1){
        return 1;
    }
    else{
        return n * factorial(n - 1);
    }
}

void main(){
    
    int number;

    printf("--FACTORIALS--\n");
    printf("Enter a number: \n");
    scanf("%d", &number);

    printf("Its factorial = %d", factorial(number));

}