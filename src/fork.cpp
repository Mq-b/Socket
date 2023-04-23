#include<iostream>
#include<unistd.h>

int gval = 10;

int main() {
	pid_t pid{};
	int lval = 20;
	gval++, lval += 5;
	pid = fork();
	if (pid == 0)
		gval += 2, lval += 2;
	else 
		gval -= 2, lval -= 2;
	if (pid == 0)
		printf("Child Proc: [%d, %d]\n", gval, lval);
	else
		printf("Parent Proc: [%d, %d]\n", gval, lval);
}