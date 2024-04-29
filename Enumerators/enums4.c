#include <stdio.h>

enum State {WORKING = 0, FAILED, FREEZED};

enum State currentChoseState = 1;

enum State findState(){
    return currentChoseState;
}

int main(){
    (findState() == WORKING) ? printf("State chosen is WORKING") : printf("State chosen is NOT working"); 
}