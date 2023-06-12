#include <stdio.h>
#include <stdlib.h>

int main()
{
    char letter;

 printf("^^^lower or upper case^^^\n");
 printf("Enter a letter:");
 scanf("%c", &letter);

 if(letter >= 'A' && letter <= 'Z'){
    printf("THIS %c is in uppercase", letter);
 }else if(letter >= 'a' && letter <= 'z'){
   printf("THIS %c is in lowercase", letter);
 }
 return 0;
}


