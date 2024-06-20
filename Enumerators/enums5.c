#include<stdio.h>

enum week{Mon=10, Tue, Wed, Thur, Fri=10, Sat=16, Sun};

enum day{Mond, Tues, Wedn, Thurs, Frid=18, Satu=11, Sund};


int main() {

printf("The value of enum week: %d\t%d\t%d\t%d\t%d\t%d\t%d\n\n",Mon , Tue, Wed,
Thur, Fri, Sat, Sun);

printf("The default value of enum day: %d\t%d\t%d\t%d\t%d\t%d\t%d",Mond , Tues,
Wedn, Thurs, Frid, Satu, Sund);


return 0;
}