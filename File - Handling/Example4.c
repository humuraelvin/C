//Example 4: Read from a binary file using fread()

#include <stdlib.h>
#include <stdio.h>

struct threeNum
{
    int n1, n2, n3;
};

int main(){

    int n;
    struct threeNum num;
    FILE *fptr;

    if ((fptr = fopen("/home/humura/Documents/workings/c_file_handling.bin", "rb")) == NULL)
    {
        printf("Error Opening the file! -- May be doesnt exist");
        exit(1);
    }

    for ( n = 0; n < 5; ++n)
    {
        fread(&num, sizeof(struct threeNum), 1, fptr);
        printf("---In the Binary File----\n");
        printf("n1: %d\t n2: %d\t n3: %d\n", num.n1, num.n2, num.n3);
    }
    
    fclose(fptr);

    return 0;
}

// In this program, you read the same file program.bin and loop through the records one by one.

// In simple terms, you read one threeNum record of threeNum size from the file pointed by *fptr into the structure num.

// You'll get the same records you inserted in Example 3.