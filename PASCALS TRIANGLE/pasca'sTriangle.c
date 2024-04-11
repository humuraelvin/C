#include <stdlib.h>
#include <stdio.h>

int main() {
    int r, c;
    int height;
    int pt[10][10];

    printf("----Pascal's Triangle----\n");

    printf("Enter the height of Pascal's triangle that you need: \n");
    scanf("%d", &height);

    printf("\n") ;     
    for (r = 0; r < height; r++) {
        for (c = 0; c <= r; c++) {
            if (c == 0 || c == r) {
                pt[r][c] = 1; 
            } else {
                pt[r][c] = pt[r-1][c] + pt[r-1][c-1]; 
            }
        }
    }

    
    for (r = 0; r < height; r++) {
        for (c = 0; c <= r; c++) {
            printf("%d ", pt[r][c]);
        }
        printf("\n");
    }

    return 0;
}
