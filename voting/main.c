#include <stdio.h>
#include <stdlib.h>

int main()
{
    int age;
    int nationalityNumber;

    printf("*****VOTING VERIFICATION******\n");
    printf("range of ages\n");
    printf("> 0-17\n");
    printf(">18\n");
    printf(">18+\n");

    printf("NATIONALITY OPtions\n");
    printf("1.Rwandan\n");
    printf("2.Other\n");

    printf("Enter your age: \n");
    scanf("%d", &age);
    printf("Enter your nationality option number 1(Rwandan) or 2(Other): \n");
    scanf("%d", &nationalityNumber);

    if(age >= 18 && nationalityNumber == 1){
        printf("YOU ARE allowed to vote");
    }
    else{
        printf("----->YOU are not allowed to vote");
    }

    return 0;
}

