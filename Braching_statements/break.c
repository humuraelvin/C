#include <stdio.h>
#include <stdlib.h>

int main(){
    int counter = 0;
    while (counter < 8){
    printf("%d", counter);
    if (counter == 4)
    break;
    counter += 1;
    }
}