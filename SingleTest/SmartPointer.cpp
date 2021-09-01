#include "./../TestInterface.h"


#ifdef SmartPoint

int test_interface(int argc, char** argv)
{
	int* a = new int(1);
	shared_ptr<int> sp(a);

	delete a;

	std::cout << *sp << std::endl;
	return 0;
}


#endif // SmartPoint
