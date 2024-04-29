#include <stdio.h>

enum monthsOfTheYear{January, February, March, April, May, June, July, August, September, October, November, December};

int main(){
    
    int i;
    for (i = January; i <= December; i++)
    {
        printf("%d", i);
        printf("\n");
    }
    

}