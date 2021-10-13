#include "./../TestInterface.h"

#include <iostream>

using namespace std;

template<typename T>
T maxT(T x, T y)
{
	if (y < x)
	{
		return x;
	}
	return y;
}

template<typename T>
class ClassA
{
public:
	T t;
};

template<int size>
class ClassB
{
public:
	char str[size];
};

/*
int test_interface(int argc, char** argv)
{
	cout << maxT(1.0f, 2.0f) << endl;

	ClassA<int> classA;
	classA.t = 4;
	cout << classA.t + 1 << endl;

	ClassB<3> classB;
	classB.str[0] = 'h';
	classB.str[1] = 's';
	classB.str[2] = 0;
	cout << classB.str << endl;

	return 0;
}
*/