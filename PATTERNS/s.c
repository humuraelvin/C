#include <stdio.h>

int main(){
    
    int rows = 4;

    for(int i=rows; i>=1; i--){
        for(int j=1; j<=i; j++){
            printf("*");
        }
     printf("\n");
    }
  
}