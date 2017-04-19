#include "./../TestInterface.h"

#ifdef TypeId

#include <iostream>

using namespace std;

class Base
{

};

int test_interface(int argc, char ** argv)
{
	cout << typeid(Base).name() << endl;
	return 0;
}

#endif



