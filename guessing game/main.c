#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("-----GUESSING GAME-----\n");
    int number;
    int secretNumber = 5;
    int guesscount=0;
    int guesslimit = 3;


       while(guesscount < guesslimit ){
       if(number != secretNumber){
        printf("Enter a number that you guess it is the secret: \n");
        scanf("%d", &number);
        guesscount++;


           if(secretNumber == number){
        printf("CONGZ!!!! YOU WIN");
    }
     else if(guesscount > guesslimit){
            printf("You loose play again");
        }

       }
    }

    return 0;
}
