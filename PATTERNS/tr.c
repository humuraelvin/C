#include <stdlib.h>
#include <stdio.h>



int main(){

    int i, j, k;


    for ( i = 0; i <= 5; i++)
    {
        for ( j = 5; j >= i; j--)
        {
            printf(" ");
        }
        for ( k = 1; k <= i; i++)
        {
            printf("*");
        }
        
        
    }

    printf("\n");


    return 0;
    

}