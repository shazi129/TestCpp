#include "./FactoryPattern.h"

#ifdef FactoryPattern

Factory::Factory(){}

Factory::~Factory(){}

Factory * Factory::shareInstance()
{
	static Factory factory;
	return &factory;
}

void Factory::addClass(const string & key, Base * baseClass)
{
	map<string, Base*>::iterator iter = mClassMap.find(key);
	if (iter != mClassMap.end())
	{
		delete (iter->second);
		mClassMap.erase(iter);
	}

	mClassMap[key] = baseClass;
}

int test_interface(int argc, char ** argv)
{
	Factory::shareInstance()->getClass<DeriveA>()->print();
	Factory::shareInstance()->getClass<DeriveB>()->print();
	return 0;
}

#endif