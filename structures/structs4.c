#include <stdio.h>

struct Point
{
    int x, y;
};

int main(){
    struct Point str = {1,2};

    struct Point* ptr = &str;

    printf("%d %d", ptr->x, ptr->y);

    return 0;
    
}
