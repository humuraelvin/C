#include <stdio.h>
#include <stdlib.h>

int main(){
  int secretnumber=6;
  int guess;
  int guesscount=0;
  int guesslimit=4;
  int outofguesses=0;

  printf("*******GAME*********\n");
  printf("------GUESS THE SECRET NUMBER--------\n");
  printf("HINT ; NUMBER btn 0 and 13\n");
   while(guess !=secretnumber && outofguesses==0){
    if(guesscount<guesslimit){
        printf("Enter a number that YOU think it is the secret \n ");
        scanf("%d", &guess);
        guesscount++;

        }else {
            outofguesses = 1;
        }

   }

  if(outofguesses==1){
    printf("You loose !!!out of guesses");
  }
    else{
        printf("YOU WIN!!!! YOU've guessed the secret number");
  }


 return 0;
}
