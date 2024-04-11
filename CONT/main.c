#include <stdio.h>
#include <stdlib.h>

int main()
{
    int day;

    printf("ENTER A (1-7) NUMBER OF THE DAY OF WEEK: \n");
    scanf("%d", &day);

    switch(day){


    case 1 :
    printf("MONDAY\n");
    break;

    case 2 :
    printf("TUESDAY\n");
    break;

    case 3 :
    printf("WEDNESDAY\n");
    break;

    case 4 :
    printf("THURSDAY\n");
    break;

    case 5 :
    printf("FRIDAY\n");
    break;

    case 6 :
    printf("SATURDAY\n");
    break;

    case 7 :
    printf("SUNDAY\n");

    default:
        printf("INVALID DAY OF THE WEEK");
        break;
    }
    return 0;
}
