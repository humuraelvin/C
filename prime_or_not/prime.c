#include <stdio.h>

int main(){
    int n;
    int lv;
    int flag = 1;

    printf("Enter a number: \n");
    scanf("%d", &n);


    if (n < 2)
    {
        printf("The entered number is not a prime");
        return 0;
    }

   for ( lv = n-1; lv >=2; lv--)
   {
      if (n % lv == 0)
      {
        flag = 0;
        break;
      }
      
   }

   if (flag == 0)
   {
    printf("The entered number is not prime");
   }else
   {
    printf("The entered number is prime");
   }

;}