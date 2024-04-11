#include <stdio.h>
#include <stdlib.h>

void multidimensionalArrayMan(int arr[][10][10], int x, int y, int z) {
    for (int k = 0; k < z; k++) {
        for (int j = 0; j < y; j++) {
            for (int i = 0; i < x; i++) {
                printf("The number %d is [%d][%d][%d]\n", arr[i][j][k], i, j, k);
            }
        }
    }
}

int main() {
    printf("-----Multidimensional *(3D)* Array Manipulation-----\n");
    int x, y, z;
    printf("Enter the value of x in the 3D array: \n");
    scanf("%d", &x);
    printf("Enter the value of y in the 3D array: \n");
    scanf("%d", &y);
    printf("Enter the value of z in the 3D array: \n");
    scanf("%d", &z);

    int arr[10][10][10];

    for (int k = 0; k < z; k++) {
        for (int j = 0; j < y; j++) {
            for (int i = 0; i < x; i++) {
                printf("Please enter element number [%d][%d][%d]: ", i, j, k);
                scanf("%d", &arr[i][j][k]);
            }
        }
    }

    printf("Printing the array as entered:\n");
    multidimensionalArrayMan(arr, x, y, z);

    return 0;
}
