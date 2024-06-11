#include <stdlib.h>
#include <stdio.h>

int main(){
    int rows = 4;
    
    for(int i = 1; i <= rows; i++){
        for(int space = 1; space <= i; space++){
            printf(" ");
        }
        for(int j = rows; j >= i; j--){
            printf("*");
        }
        printf("\n");
    }

}