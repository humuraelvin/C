#include <stdio.h>

struct str1
{
	int i;
	char c;
	float f;
	char s[30];
};

struct str2
{
	int ii;
	char cc;
	float ff;
} var;

int main(){

	struct str1 var1 = {1, 'A', 0.976, "Rwanda"}, var2;

	struct str2 var3 = {.ff=2.00, .cc='B', .ii=50};

	var1 = var2;

	printf("Structure 1: \n\t i = %d, c=%c, f=%f, s=%s \n", var1.i, var1.c, var1.f, var1.s);

	printf("Structure 2: \n\t i = %d, c=%c, f=%f, s=%s \n", var2.i, var2.c, var2.f, var2.s);

	printf("Structure 3: \n\t i = %d, c=%c, f=%f \n", var3.ii, var3.cc, var3.ff);

}

