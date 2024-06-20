#include <stdlib.h>
#include <stdio.h>



int main(){

    int i, j, k;


    for ( i = 0; i <= 5; i++)
    {
        for ( j = 0; j >= i; j++)
        {
            printf(" ");
        }
        for ( k = 0; k <= i; i++)
        {
            printf("*");
        }
        
        
    }

    printf("\n");


    return 0;
    

}