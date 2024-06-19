#include <stdio.h>
#include <stdlib.h>

void bubbleSort(int arr[], int n){
    int i, j, temp;

    for ( i = 0; i < n-1; i++)
    {
        for (j = 0; j < n - i -1; j++)
        {
            if (arr[j] > arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
            
        }
        
    }
    
}

void printArray(int arr[], int size){
    int i;

    for ( i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
    }
    printf("\n");

}

int main(){

    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr)/sizeof(arr[0]);

    printf("Unsorted array: \n");
    printArray(arr, n);
    
    bubbleSort(arr, n);
    
    printf("Sorted array: \n");
    printArray(arr, n);
    
    return 0;

}
