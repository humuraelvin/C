//Differences between unions and structures demonstration

#include <stdio.h>

union unionJob
{
    char name[32];
    float salary;
    int workerNo;
} uJob;

struct structJob
{
    char name[32];
    float salary;
    int workerNo;
}sJob;

int main(){
    printf("The size of a union is %d bytes \n", sizeof(uJob));
    printf("The size of a structure is %d bytes", sizeof(sJob));
}

//The Output will be
//The size of a union is 32 bytes 
//The size of a structure is 40 bytes 

// Why this difference in the size of union and structure variables?

//Here, the size of sJob is 40 bytes because

//the size of name[32] is 32 bytes
//the size of salary is 4 bytes
//the size of workerNo is 4 bytes
//However, the size of uJob is 32 bytes. It's because the size of a union variable
// will always be the size of its largest element. In the above example,
// the size of its largest element, (name[32]), is 32 bytes.

