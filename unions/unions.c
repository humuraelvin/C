#include <stdio.h>

//Definition of a union
    union car{
        char name[50];
        int price;
    };



int main(){
    
    //When a union is defined, it creates a user-defined type. However, no memory is allocated.
    // To allocate memory for a given union type and work with it, we need to create variables.
    //Here's how we create union variables.
    union car car1, car2, *car3;

  return 0;
};

//Accessing members of a union
//We can use . to access mebers of a unions
//We can use -> to acess pointer variables of unions

//In the above example,

//To access price for car1, car1.price is used.
//To access price using car3, either (*car3).price or car3->price can be used.

