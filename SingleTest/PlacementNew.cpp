
#include "../TestInterface.h"

#ifdef TEST_PLACEMENT_NEW

#include <iostream>
#include <vector>

using namespace std;

class Person
{
public:
	Person()
	{
		cout << "Person default constructor" << endl;
		_name = "unknow";
		_age = 0;
	}

	Person(const Person& other)
	{
		cout << "Person copy constructor" << endl;
		_name = other._name;
		_age = other._age;
	}

	Person(const Person&& other)
	{
		cout << "Person move constructor" << endl;
		_name = other._name;
		_age = other._age;
	}

	Person(string name, int age)
	{
		cout << "Person custom constructor" << endl;
		_name = name;
		_age = age;
	}

	void Print()
	{
		cout << _name << "  " << _age << endl;
	}

private:
	int _age;
	string _name;
};

void* operator new(size_t Size, vector<Person>& vec)
{
	vec.push_back(Person());
	return &vec[vec.size() - 1];
}


int test_interface(int argc, char** argv)
{
	vector<Person> persons(1);

	new(&persons[0])Person("aaaa", 35);
	
	Person* p = new(persons)Person("bbbb", 33);

	for (unsigned i = 0; i < persons.size(); i++)
	{
		persons[i].Print();
	}

	return 0;
}

#endif