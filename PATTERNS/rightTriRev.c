#include <stdlib.h>
#include <stdio.h>

int main(){
    int rows = 5;
    int i;
    int j;
    int space;
    
   for(i=0; i<=rows; i++){
    for(space=1; space<=rows -i; space++){
        printf(" ");
    }
    for(j=0; j<=i; j++){
        printf("*");
    }
    printf("\n");
   }
   
}