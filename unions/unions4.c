// C program to check if we can store data in multiple union

#include <stdio.h>

union test {
	int x, y;
};

int main()
{
	
	union test t;

	t.x = 2; 
	printf("After making x = 2:\n x = %d, y = %d\n\n", t.x,
		t.y);

	t.y = 10; // t.x is also updated to 10
	printf("After making y = 10:\n x = %d, y = %d\n\n", t.x,
		t.y);
	return 0;
}
