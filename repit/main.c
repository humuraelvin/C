#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){

 int num=1;

 int multipleof3 = (num%3==0);

 int multipleof5 = (num%5==0);

 int multipleof15 = (num%15==0);

 while(num<=100){

  printf("%d\n", num);

 num++;
 
  if(num==multipleof3){
        printf("Fizz");
    }
    else if(num==multipleof5){
        printf("Buzz");
    }
    else if(num==multipleof15){
        printf("FizzBuzz");
    }
 }

 return 0;
}
