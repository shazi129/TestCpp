#include <iostream>
using namespace std;

class Base
{
public:
	Base()
	{
		cout << "base construct" << endl;
		str = new char[12];
	}
	virtual ~Base()
	{
		cout << "base deconstruct" << endl;
		delete[] str;
	}
private:
	char * str;
};

class Derive : public Base
{
public:
	Derive()
	{
		cout << "Derive construct" << endl;
	}
	~Derive()
	{
		cout << "Derive deconstruct" << endl;
	}
private:
	Base base;
};

int main()
{
	// void * voidBase = new Base();
	// delete voidBase;

	void * voidDerive = new Derive();
	delete voidDerive;

	// Base * pBase = new Base();
	// delete pBase;

	// Base * pBase = new Derive();
	// delete pBase;

	return 0;
}