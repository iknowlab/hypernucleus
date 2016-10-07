#include <iostream>
#include <stdlib.h>
int main(int argc, char **argv){
	double Step,Range;
	Step =atof(argv[1]);
	Range =atof(argv[2]);
	for(double i=Step*2;i<=Range;i+=Step){
	std::cout << i << "." <<std::endl;	
	}//for
return 0;
}//main
