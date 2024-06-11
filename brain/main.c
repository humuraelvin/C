#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main() {
    int num, sum = 0;

    printf("Enter numbers to add (enter -1 to exit):\n");

    while (1) {
        printf("Enter a number: ");
        scanf("%d", &num);

        if (num == -1) {
            break;
        }

        sum += num;
    }

    printf("Sum of the numbers: %d\n", sum);

    return 0;
}

