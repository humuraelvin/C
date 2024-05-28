//typedef keyword

#include <stdio.h>

struct str1{
    int a;
    float b;
};

typedef struct str1 str1;

typedef struct str2{
    int x;
}str2;


int main(){
    str1 var1 = { 20 };
    str2 var2 = { 314 };

    printf("var1.a = %d \n", var1.a);
    //printf("var1.b = %f \n", var1.b);

    printf("var2.x = %d \n", var2.x);


    return 0;
}
