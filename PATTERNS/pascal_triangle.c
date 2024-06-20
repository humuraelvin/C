#include <stdio.h>

int main() {
    int height, c, r, pt[10][10];

    printf("Enter the height of the triangle: ");
    scanf("%d", &height);

    for (r = 0; r < height; r++) {
        for (c = 0; c <= r; c++) {
            if (c == 0 || c == r) {
                pt[r][c] = 1;  
            } else {
                pt[r][c] = pt[r-1][c-1] + pt[r-1][c];
            }
            printf("%d ", pt[r][c]);
        }
        printf("\n");
    }

    return 0;
}
