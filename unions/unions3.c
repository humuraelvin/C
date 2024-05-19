//accessing union data be like
#include <stdio.h>

union Job
{
    float salary;
    int workerNo;
} j;

int main(){
    j.salary = 12.3;

    // when j.workerNo is assigned a value,
   // j.salary will no longer hold 12.3
   j.workerNo = 100;

   printf("Salary = %.1f \n", j.salary);
   printf("Number of Workers = %d \n", j.workerNo);

}

//Output
//Salary = 0.0 
//Number of Workers = 100 