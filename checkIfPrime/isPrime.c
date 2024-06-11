#include <stdio.h>

int isPrime(int num){
    if (num <= 1)
    {
        return 0;
    }
    for (int i = 2; i * i <=num; i++)
    {
        if (num % i == 0)
        {
            return 0;
        }
        
    }

    return 1;
    
}

int main(){

   int number;
    printf("----CHECKING IF PRIME----\n");
    printf("Enter a positive integer: \n");
    scanf("%d", &number);

    if (isPrime(number))
    {
        printf("The number you inputed is a prime.");
    }else
    {
        printf("The number you inputed  is not a prime");
    }
    
    

}