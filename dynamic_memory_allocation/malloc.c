//C malloc()
//The name "malloc" stands for memory allocation.

//The malloc() function reserves a block of memory of the specified number of bytes.
// And, it returns a pointer of void which can be casted into pointers of any form.

#include <stdio.h>



int main(){

    //syntax
    //double ptr = (float*) malloc(100 * sizeof(float));

    return 0;
}

//The above statement allocates 400 bytes of memory. It's because the size of float is 4 bytes.
// And, the pointer ptr holds the address of the first byte in the allocated memory.

//The expression results in a NULL pointer if the memory cannot be allocated.
