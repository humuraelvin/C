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

    if ((fptr = fopen("/home/humura/Documents/workings/c_file_handling.bin","wb")) == NULL)
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

// In this program, we create a new file program.bin in the C drive.

// We declare a structure threeNum with three numbers - n1, n2 and n3, and define it in the main function as num.

// Now, inside the for loop, we store the value into the file using fwrite().

// The first parameter takes the address of num and the second parameter takes the size of the structure threeNum.

// Since we're only inserting one instance of num, the third parameter is 1. And, the last parameter *fptr points to the file we're storing the data.

// Finally, we close the file.