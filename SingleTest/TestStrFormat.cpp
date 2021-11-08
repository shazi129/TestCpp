#include "../TestInterface.h"

#ifdef TEST_STR_FORMAT

#include <stdio.h>
#include <iostream>
using namespace std;



int test_interface(int argc, char** argv)
{
	char a1[10] = { 'A', 'B', 'C', 'D', 'E'};
	printf("%.2s", a1);
	return 0;
}

#endif