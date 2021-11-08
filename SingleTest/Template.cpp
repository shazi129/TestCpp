#include "../TestInterface.h"

#ifdef TEST_TEMPLATE

#include <iostream>

using namespace std;

//ģ�庯��
template<typename T>
T maxT(T x, T y)
{
	if (y < x)
	{
		return x;
	}
	return y;
}

//ģ����
template<typename T>
class ClassA
{
public:
	T t;
};

//����ģ����
template<int size>
class ClassB
{
public:
	char str[size];
};

class TestClass
{
public:
	virtual void Print() = 0;
};

template <typename T>
struct TIsAbstract
{
	enum { Value = __is_abstract(T) };
};


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

	cout << __is_abstract(TestClass) << endl;

	return 0;
}

#endif
