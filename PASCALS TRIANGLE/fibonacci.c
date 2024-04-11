#include <stdio.h>
#include <stdlib.h>

int fibonacci(int a){
    if(a==0){
        return 0;
    }else if(a == 1){
        return 1;
    }else{
        return fibonacci(a-1) + fibonacci(a-2);
   }
}

void main(){

    int n, i;

    printf("---FIBONNACCI SERIES----\n");
    printf("Enter n: \n");
    scanf("%d", &n);
    printf("\n");
    for (i=0; i<n; i++){
        printf("%d", fibonacci(i));
        if(i != n-1){
            printf(", ");
        }
    }
    printf("\n");

}