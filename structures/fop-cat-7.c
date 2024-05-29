#include<stdio.h>

void keyValue(int, char);

union abc
{
    int a;
    char b;
}var;

struct charSet
{
    int i;
    int s;
}cs;

int main(){
    var.a = 5;
    printf("Member a= %d, member b = %c, size of abc union = %d \n", var.a, var.b,sizeof(var));
    keyValue(var.a, var.b);
    printf("Member s= %d, member g = %c, size of cs structure = %d \n", cs.i, cs.s,sizeof(cs));
    return 0;
}

void keyValue(int charValueOne, char charValueTwo){
    cs.i = charValueOne;
    cs.s = charValueTwo;
}


