#include <stdio.h>
#include <stdlib.h>

int main()
{
    int num;
    printf("ENTER AN INTEGER: \n");
    scanf("%d", &num);

    if(num == 0){
        printf("this integer is neither positive nor negative");
    }
    else if(num > 0){
        printf("this integer is positive");

    }
    else if(num < 0){
        printf("this integer is negative");
    }
    return 0;
}
