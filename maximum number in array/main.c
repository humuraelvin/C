#include <stdio.h>
#include <stdlib.h>

int main()

{
    system("color C5");
   int n;
    printf("=====CALCULATE MAX=======\n");

    printf("Enter the number of elements:");
    scanf("%d",&n);

    int myArray[n];
    int number;
    int maximum;
    int index;

    printf("*****MAXIMUM NUMBER IN ARRAY*****\n");
    for(index = 0; index < n; index++){
        printf("ENTER A NUMBER : \n");
        scanf("%d", &myArray[index]);
    }
    maximum=myArray[0];
    int length=sizeof(myArray)/sizeof(int);
    for(index = 0; index < length; index ++){
        if(myArray[index] > maximum){
            maximum = myArray[index];
        }
    }
    printf("THE maximum number among the above is %d",maximum);

    return 0;
}
