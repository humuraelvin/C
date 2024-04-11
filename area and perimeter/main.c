#include <stdio.h>
#include <stdlib.h>

int perimeter(length, width){
  return(2*(length+width));
}
int area(length, width){
  return(length*width);
}

int main()
{
    double length, width, perimeter, area, functionnumber;
    printf("----AREA AND PERIMETER OF RECTANGLE----\n");
    printf("functions with numbers\n");
    printf("1.PERIMETER\n");
    printf("2.AREA\n");

    printf("Enter the length\n");
    scanf("%lf", &length);
    printf("Enter the width\n");
    scanf("%lf", &width);
    printf("Enter the function number(1or2)\n", functionnumber);
    scanf("%lf", &functionnumber);

    perimeter=2*(length+width);
    area=length*width;
    if(functionnumber==1){
        printf("The perimeter of the rectangle is %lf", perimeter);
    }
    else if(functionnumber==2){
        printf("The area of the rectangle is %lf", area);
    }




    return 0;
}
