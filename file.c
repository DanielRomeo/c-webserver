#include <stdio.h>
#include <stdlib.h>

extern "C" void f(int);

void f(int i){
	std::cout << "f called" << std::endl;
}

int main(){

	// FILE *fpointer;

	

	return 0;
}