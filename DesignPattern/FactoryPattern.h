#include "./../TestInterface.h"

#ifdef FactoryPattern

#include <string>
#include <map>

class Base
{
public:
	virtual void print() {cout << "this is class Base" << endl;}
};

class DeriveA : public Base
{
public:
	virtual void print() {cout << "this is class DeriveA" << endl;}
};

class DeriveB : public Base
{
public:
	virtual void print() {cout << "this is class DeriveB" << endl;}
};

class Factory
{
public:
	static Factory * shareInstance();
	~Factory();

	template<typename T>
	T* getClass()
	{
		string keyName = string(typeid(T).name());

		map<string, Base*>::iterator iter = mClassMap.find(keyName);
		if (iter != mClassMap.end())
		{
			T *  retObj = dynamic_cast<T*>(iter->second);
			if (retObj)
			{
				return retObj;
			}
		}

		T* retObj = new T();
		addClass(keyName, retObj);
		return retObj;
	}

private:
	Factory();
	void addClass(const string & key, Base * baseClass);

private:
	map<string, Base*> mClassMap;
};

#endif