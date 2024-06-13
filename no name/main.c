#include <stdio.h>

int main() {
    int count, num;
    float sum = 0, average;

    printf("Enter the count of numbers: ");
    scanf("%d", &count);

    for (int i = 1; i <= count; i++) {
        printf("Enter number %d: ", i);
        scanf("%d", &num);
        sum += num;
    }

    average = sum / count;

    printf("Sum = %.2f\n", sum);
    printf("Average = %.2f\n", average);

    return 0;
}

