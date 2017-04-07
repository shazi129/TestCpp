#include "./../TestInterface.h"

#ifdef Convert2Https

#include <iostream>
#include <vector>
#include <string>

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

class UrlInfo
{
public:
	string protoPrefix; //Э��ǰ׺����http��https��ftp��
	string hostName; //����
	string requestPath; //����
	int port;  //url�еĶ˿ڣ����Ϊ-1����ʾû������

	UrlInfo()
	{
		protoPrefix = "";
		hostName = "";
		requestPath = "";
		port = 0;
	}
};

int getUrlInfo(const string & url, UrlInfo & urlInfo)
{
	if (url.empty())
	{
		return -1;
	}

	int anchor = 0;
	int index = url.find("://");
	if (index != string::npos)
	{
		urlInfo.protoPrefix = url.substr(anchor, index);
		anchor = index + 3; //������://��
	}

	index = url.find("/", anchor);
	if (index != string::npos)
	{
		string hostPort = url.substr(anchor, index - anchor);
		int portPos = hostPort.find(":");
		if (portPos != string::npos)
		{
			urlInfo.hostName = hostPort.substr(0, portPos);
			urlInfo.port = atoi(hostPort.substr(portPos+1).c_str());
		}
		else
		{
			urlInfo.hostName = hostPort;
		}
		anchor = index;
	}

	urlInfo.requestPath = url.substr(anchor);

	return 0;
}

bool isIpV4(const string & ipStr)
{
	//1.�ָ�
	vector<string> ipsecs;
	int anchor = 0;
	int index = ipStr.find_first_of(".", anchor);
	while(index != string::npos)
	{
		ipsecs.push_back(ipStr.substr(anchor, index-anchor));
		anchor = index + 1;
		index = ipStr.find_first_of(".", anchor);
	}
	if (anchor < ipStr.size())
	{
		ipsecs.push_back(ipStr.substr(anchor));
	}

	//3. �ǲ�����4��
	if (ipsecs.size() != 4)
	{
		return false;
	}

	//2. �鿴ÿһ���Ƿ���0-255������
	for (unsigned i = 0; i < ipsecs.size(); i++)
	{
		string & item = ipsecs.at(i);

		int sum = 0;
		int weight = 1;
		for (int j = item.size() - 1; j >= 0; j--)
		{
			char & charactor = item.at(j);
			if ('0' <= charactor && charactor <= '9')
			{
				sum += (charactor - '0') * weight;
				weight *= 10;
			}
			else
			{
				return false;
			}
		}
		if (sum > 255)
		{
			return false;
		}
	}

	return true;
	
}


int test_interface(int argc, char **argv)
{
	//convert2Https();
	//UrlInfo urlInfo;
	//getUrlInfo("http://dlied5.qq.com/HeroGame/test/BFile/20170221095556/all_table_server.zip", urlInfo);
	//cout << urlInfo.protoPrefix << "\t" << urlInfo.hostName << "\t" << urlInfo.port << "\t" << urlInfo.requestPath << endl;

	cout << isIpV4("192.168.1..") << endl;

	return 0;
}

#endif