#include <stdio.h>

int main() {
	double balance[5] = {1000.0, 23.4, 60.8, 45.0};
	double* a;
	a = balance;

	printf("a = %d\n", a);
	printf("&a = %d\n", &a);
	printf("*a = %f\n", *a);
	printf("float a = %f\n", a - 1);

	return 0;
}