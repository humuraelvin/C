#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
 int num=1;
 while(num<=100){

  if(num % 3 == 0){
        printf("Fizz\n");
    }
    else if(num % 5 == 0){
        printf("Buzz\n");
    }
    else if(num % 3 == 0 && num % 5 == 0){
        printf("FizzBuzz\n");
    }else{
      printf("%d\n", num);
    }

  num++;
 }

 return 0;
}
