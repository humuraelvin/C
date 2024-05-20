//Example 3: Write to a binary file using fwrite()

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

    if ((fptr = fopen("/home/humura/Documents/workings/program.bin","r")) == NULL)
    {
        printf("Error! Opening file-- may be does not exist");  
        exit(1); 
    }

    for ( n = 1; n < 5; ++n)
    {
        num.n1 = n;
        num.n2 = 5*n;
        num.n3 = 5*n+1;
        fwrite(&num, sizeof(struct threeNum), 1, fptr);
    }
    
    fclose(fptr);

    return 0;
}
