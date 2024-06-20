#include <stdio.h>
#include <stdlib.h>

int main(){

    int height, c, r, pt[10][10];

   

    printf("Enter the height you wish: \n");
    scanf("%d", &height);
    
     for ( r = 0; r < height; r++)
    {
        for ( c = 0; c < height; c++)
        {
            if (c <= r)
            {
                if (c == r)
                {
                    pt[r][c] = 1;
                }else
                {
                    pt[r][c] = pt[r-1][c] + pt[r-1][c-1];
                }
                
                
            }
            printf("\n");
        }
        
    }


}