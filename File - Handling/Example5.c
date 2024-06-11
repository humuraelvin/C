//Example 5: fseek()

#include <stdio.h>
#include <stdlib.h>

struct threeNum
{
    int n1, n2, n3;
};


int main(){
    int n;
    struct threeNum num;
    FILE *fptr;

    if ((fptr = fopen("/home/humura/Documents/workings/program.bin","rb")) == NULL)
    {
        printf("Error! Accessing file -- May be doesnt exist");
        exit(1);
    }

    fseek(fptr, -sizeof(struct threeNum), SEEK_END);
    
    for ( n = 0; n < 5; ++n)
    {
        fread(&num, sizeof(struct threeNum), 1, fptr);
        printf("----From the Binary file ---\n");
        printf("n1: %d\t, n2: %d\t, n3: %d\n", num.n1, num.n2, num.n3);
        fseek(fptr, -2*sizeof(struct threeNum), SEEK_CUR);
    }

    fclose(fptr);
    
    return 0;
}

// This program will start reading the records from the file program.bin
//  in the reverse order (last to first) and prints it.