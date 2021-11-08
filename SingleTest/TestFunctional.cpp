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


int test_interface(int argc, char** argv)
{
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