#include <stdlib.h>
#include <stdio.h>


int main(){
    
    int n, i ,j, sum;

    printf("To which number do you want to calculate the sum of even numbers \n");
    scanf("%d", &n);

    for ( i = 0; i < n; i++)
    {
        if (i % 2 == 0)
        {
            sum += i;
            j+=1;
        }
        
    }

    int average = sum / j;

    printf("SUM = %d \n", sum);
    printf("AVERAGE = %d", average);
    
    

}