#include "./../TestInterface.h"

#include <iostream>
#include <vector>

using namespace std;

class Base
{
public:
	void FuncA() { cout << "this is Base::FuncA" << endl; }
};

class Derive : public Base
{
public:
	void FuncA() { cout << "this is Derive::FuncA" << endl; }

	void FuncB() { cout << "this is Derive::FuncB" << endl; }
};

/*
int test_interface(int argc, char ** argv)
{
	Base* base = new Derive();

	Derive* derive = static_cast<Derive*>(base);

	derive->FuncA();

	return 0;
}
*/



