//Example 2: Read from a text file

#include <stdio.h>
#include <stdlib.h>

int main(){

    int num;
    FILE *fptr;

    if ((fptr = fopen("/home/humura/Documents/workings/workings/font family.txt","r")) == NULL)
    {
        printf("Error! opening file -- May be doesnt exist in specified path");
        exit(1);
    }

    fscanf(fptr,"%d", &num);

    printf("Value of n = %d", num);
    fclose(fptr);
    
    return 0;
}