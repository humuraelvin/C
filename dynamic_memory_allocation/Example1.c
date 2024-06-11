//Example 1: malloc() and free()

//program to calculate the sum of n numbers entered by the user

#include <stdio.h>
#include <stdlib.h>

int main(){

    int n, i, *ptr, sum = 0;

    printf("Enter the number of elements: ");
    scanf("%d \n", &n);

    ptr = (int*) malloc(n * sizeof(int));

    if (ptr == NULL)
    {
        printf("Error! memory not allocated ");
        exit(0);
    }

    printf("Enter elements: \n");
    for (int i = 0; i < n; ++i)
    {
        scanf("%d", ptr + i);
        sum += *(ptr + i);
    }

    printf("Sum = %d", sum);

    free(ptr);
    
    return 0;
}