#include <stdio.h>
#include <stdlib.h>

int main()
{
    char celebrity [20];
    char boyinyear1 [20];
    char pluralnoun [20];
    char proffesion [20];
    int favnum;
    char favmusic [20];
    char favplace [20];
    char favobject [20];
    int numberabove20;

    printf("Enter a celebrity: \n");
    scanf("%s", &celebrity );
    printf("Enter a boyinyear1: \n");
    scanf("%s", &boyinyear1);
    printf("Enter a pluralnoun: \n");
    scanf("%s", &pluralnoun);
    printf("Enter a proffesion: \n");
    scanf("%s", &proffesion);
    printf("Enter a favnum: \n");
    scanf("%d\n", &favnum);
    printf("Enter a favmusic: \n");
    scanf("%s", &favmusic);
    printf("Enter a favplace: \n");
    scanf("%s", &favplace);
    printf("Enter a favobject: \n");
    scanf("%s", &favobject);
    printf("Enter a numberabove20: \n");
    scanf("%d", &numberabove20);

    printf("my favorite celebrity is {celebrity} \n");
    printf("my crash in year one is {boyinyear1}\n");
    printf("{pluralnoun} are my favs");
    printf("I would like to be a {proffession} in future\n");
    printf("%d is my favourite number\n", favnum);
    printf("{favmusic} is my favourite music\n");
    printf("my favorite place is {favplace}\n");
    printf("my favourite object is {favobject}\n");
    printf("I will get married when I'm %d year\n", numberabove20);

    return 0;
}
