#include <functional>
#include <iostream>
#include <map>

#include "./../TestInterface.h"

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

int test_interface(int argc, char** argv)
{
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