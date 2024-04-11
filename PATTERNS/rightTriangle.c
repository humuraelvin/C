#include <stdlib.h>
#include <stdio.h>

int main(){
    int i;
    int j;

    for(i=0; i<6; i++){
        for(j=0; j<i; j++){
            printf("* ");
        }
     printf("\n");
    }

}