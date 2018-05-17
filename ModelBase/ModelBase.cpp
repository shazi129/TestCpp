#include "./../TestInterface.h"

#ifdef ModelBaseTest

#include <map>
#include <iostream>
#include <vector>
#include <functional>
#include <string>

class ModelBindingInfo
{
public:
	void * data;
	vector<std::function<void(void*)>> callbacks;
};

class ModelBase
{
public:
	ModelBase()
	{

	}
	~ModelBase() 
	{
		_bindingTable.clear();
	}

	void declareObservable(string obName, void * pVar)
	{
		map<string, ModelBindingInfo>::iterator iter = _bindingTable.find(obName);
		if (iter == _bindingTable.end())
		{
			_bindingTable[obName].data = pVar;
		}
	}

	template<typename T>
	void setObservableData(string obName, T data)
	{
		map<string, ModelBindingInfo>::iterator iter = _bindingTable.find(obName);
		if (iter != _bindingTable.end())
		{
			T* pData = (T*)iter->second.data;
			*pData = data;

			for (unsigned int i = 0; i < iter->second.callbacks.size(); i++)
			{
				iter->second.callbacks[i](&data);
			}
		}
	}

	void addObserverBinding(string obName, std::function<void(void*)> cb)
	{
		map<string, ModelBindingInfo>::iterator iter = _bindingTable.find(obName);
		if (iter != _bindingTable.end())
		{
			_bindingTable[obName].callbacks.push_back(cb);
		}
	}

private:
	map<string, ModelBindingInfo> _bindingTable;
};

class Person : public ModelBase
{
public:
	Person()
	{
		declareObservable("address", &_address);
	}

private:
	string _address;
};

class Test
{
public:
	void test()
	{
		Person zhangwen;
		Person shazi;

		zhangwen.addObserverBinding("address", std::bind(&Test::onZhangwenAddrChange, this, std::placeholders::_1));
		shazi.addObserverBinding("address", std::bind(&Test::onShaziAddrChange, this, std::placeholders::_1));

		zhangwen.setObservableData<std::string>("address", "caifugang");
		shazi.setObservableData<std::string>("address", "baijin");
	}
	void onShaziAddrChange(void * obj)
	{
		cout << "shazi's addr changed, new addr: " << *(std::string*)obj << endl;;
	}

	void onZhangwenAddrChange(void * obj)
	{
		cout << "zhangwen's addr changed, new addr: " << *(std::string*)obj  << endl;
	}
};

int test_interface(int argc, char ** argv)
{
	Test test;
	test.test();
	return 0;
}

#endif

