#include <stdio.h>
#include <stdlib.h>

int main()
{
    int firstnumber,secondnumber,thirdnumber,maximumnumber;

    printf("*****COMPARING PROGRAM********\n");
    printf("Enter the first number of your choice\n");
    scanf("%d", &firstnumber);
    printf("Enter the second number of your of your choice\n");
    scanf("%d", &secondnumber);
    printf("Enter the third number of your choice\n");
    scanf("%d", &thirdnumber);

    if(firstnumber>secondnumber && firstnumber>thirdnumber){
        printf("%d is the maximum of the three that you entered", firstnumber);

    }else if(secondnumber>firstnumber && secondnumber>thirdnumber){
      printf("%d is the maximum of the three that you have entered", secondnumber);
    }else if(thirdnumber>firstnumber && thirdnumber>secondnumber){
     printf("%d is the maximum number of the three that you enterd");
    }




    return 0;

}
