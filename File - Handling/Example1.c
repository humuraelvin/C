//Example 1: Write to a text file
#include <stdio.h>
#include <stdlib.h>

int main(){
    int num;
    FILE *fptr;

    fptr = fopen("/home/humura/Documents/workings/c_file_handling.txt","a");

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