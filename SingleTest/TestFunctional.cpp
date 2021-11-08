#include "./../TestInterface.h"

#ifdef TEST_FUNCTIONAL

#include <functional>
#include <iostream>
#include <map>

using namespace std;

int add(int a, int b)
{
	return a + b;
}

auto mod = [](int a, int b)
{
	return a % b;
};

struct Divide
{
	int operator()(int a, int b)
	{
		return a / b;
	}
};

void funcWrapper(void* const ret)
{
	*(int*)ret = add(1, 2);
}

class A { public: void testA(int t) { cout << "A:TestA " << t << endl; } };
class B { public: void testB(int t) { cout << "B:TestB " << t << endl; } };

int test_interface(int argc, char** argv)
{
	
	A* a = new A();
	B* b = new B();

	//传统函数指针
	class Base {};
	typedef void (Base::* Extfunc)(int t);
	Extfunc func = (Extfunc)(&A::testA);
	(((Base*)a)->*func)(1);
	func = (Extfunc)(&B::testB);
	(((Base*)b)->*func)(2);

	//functional
	std::function<void(int)> func2 = std::bind(&A::testA, a, std::placeholders::_1);
	func2(3);
	func2 = std::bind(&B::testB, b, std::placeholders::_1);
	func2(4);


	int funcRet = 0;
	funcWrapper(&funcRet);
	cout << "funcWrapper return:" << funcRet << endl;

	map<string, function<int(int, int)>> ops
	{
		{"+", add},
		{"-", [](int a, int b) {return a - b; }},
		{"%", mod},
		{"*", std::multiplies<int>()},
		{"/", Divide()}
	};

	cout << "2+3=" << ops["+"](2, 3) << endl;
	cout << "2-3=" << ops["-"](2, 3) << endl;
	cout << "2%3=" << ops["%"](2, 3) << endl; 
	cout << "2*3=" << ops["*"](2, 3) << endl;
	cout << "2/3=" << ops["/"](2, 3) << endl;

	return 0;
}


#endif