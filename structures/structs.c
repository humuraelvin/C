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

	struct str1 var1 = {1, 'A', 0.976, 'Rwanda coding academy'}, var2;

	struct str2 var3 = {.ff=2.00, .cc='B', .ii=50};

	var1 = var2;

}

