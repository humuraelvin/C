//Example 1: Write to a text file
#include <stdio.h>
#include <stdlib.h>

int main(){
    int num;
    FILE *fptr;

    fptr = fopen("/home/humura/Documents/workings/font family.txt","w");

    if (fptr == NULL)
    {
        printf("Error , File does not exist");
        exit(0);
    }

    printf("Enter a number: ");
    scanf("%d", &num);

    fprintf(fptr,"%d",num);
    fclose(fptr);
    
}