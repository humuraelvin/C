#include <stdio.h>
#include <string.h>

int main() {
    const char *str1 = "hello";
    const char *str2 = "hello";
    const char *str3 = "world";
    const char *str4 = "hell";

    int result1 = strcmp(str1, str2);  // should return 0
    int result2 = strcmp(str1, str3);  // should return a negative value
    int result3 = strcmp(str1, str4);  // should return a positive value

    printf("Comparing '%s' and '%s': %d\n", str1, str2, result1);
    printf("Comparing '%s' and '%s': %d\n", str1, str3, result2);
    printf("Comparing '%s' and '%s': %d\n", str1, str4, result3);

    return 0;
}
