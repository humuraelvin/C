#include <stdio.h>

enum State {WORKING = 0, FAILED, FREEZED};

enum State currentChoseState = 2;

enum State findState(){
    return currentChoseState;
}

int main(){
    (findState() == FREEZED) ? printf("State chosen is WORKING") : printf("State chosen is NOT working"); 
}