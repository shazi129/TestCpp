#include "./../TestInterface.h"

#ifdef Convert2Https

#include <iostream>
#include <vector>

using namespace std;

void convert2Https()
{
	vector<string> http;
	http.push_back("http://www.baidu.com");
	http.push_back("www.baidu.com");
	http.push_back("https://www.baidu.com");
	
	for (unsigned int i = 0; i < http.size(); i++)
	{
		string & item = http.at(i);
		int index = item.find("https://");
		if (index == 0)
		{
			continue;
		}

		index = item.find("http://");
		if (index == 0)
		{
			item.insert(4, "s");
		}
		else
		{
			item.insert(0, "https://");
		}
	}

	for (unsigned int i = 0; i < http.size(); i++)
	{
		cout << http.at(i).c_str() << endl;
	}
}


int test_interface(int argc, char **argv)
{
	convert2Https();
	return 0;
}

#endif