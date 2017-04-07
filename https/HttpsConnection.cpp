#include "./../TestInterface.h"


#ifdef HttpsConnection

#include <WINSOCK2.H>
#include <WS2tcpip.h>
#include <iostream>
#include <sstream>

#include "pthread.h"
#include "./HttpsConnection.h"

using namespace std;

#define WRITE_BUFF_SIZE 1024
#define READ_BUFF_SIZE 10240


int HttpDownloader::stripHttpHeader(char * buff, int buffSize)
{
	for (int i = 0; i < buffSize-4; i++)
	{
		if (buff[i] == 0x0D
			&& buff[i+1] == 0x0A
			&& buff[i+2] == 0x0D
			&& buff[i+3] == 0x0A)
		{
			cout << "find http haader end pos:" << i+4 << endl;
			return i+4;
		}
	}
	return -1;
}

HttpDownloader::HttpDownloader()
{

}

HttpDownloader::~HttpDownloader()
{

}

void HttpDownloader::addUrl(const std::string & url)
{
	HttpDownloadUrlInfo info;
	int ret = initUrlInfoWithUrl(url, info);
	if (ret == 0)
	{
		mUrls.push_back(info);
	}
}

int HttpDownloader::initUrlInfoWithUrl(const std::string & httpUrl, HttpDownloadUrlInfo & info)
{
	string url = httpUrl;

	//1. 取掉url的http头
	int index = url.find("https://");
	if (index == 0) //找到
	{
		url = url.substr(8);
		info.needSSL = true;
		info.port = 443;
	}
	index = url.find("http://");
	if (index == 0)
	{
		url = url.substr(7);
		info.needSSL = false;
		info.port = 80;
	}

	//获取get字符串
	index = url.find("/");
	if (index == string::npos)
	{
		info.host = url;
		info.getStr = "/index.html";
	}
	else
	{
		info.host = url.substr(0, index);
		info.getStr = url.substr(index);
	}

	//获取文件名
	index = info.getStr.rfind("/");
	info.saveFileName = info.getStr.substr(index+1);

	return 0;
}

void HttpDownloader::initSSLEnv()
{
	SSL_load_error_strings ();
	SSL_library_init ();
	OpenSSL_add_all_algorithms();
}

void HttpDownloader::destroySSLEnv()
{
	sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
	FIPS_mode_set(0);
	CONF_modules_free();
	ENGINE_cleanup();
	CONF_modules_unload(1);
	ERR_free_strings();
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
}

vector<HttpDownloadUrlInfo> & HttpDownloader::getUrls()
{
	return mUrls;
}

int HttpDownloader::createSocket(HttpDownloadUrlInfo & info)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	stringstream ss;
	ss << info.port;

	info.addrResult = NULL;
	int error = getaddrinfo(info.host.c_str(), ss.str().c_str(), &hints, &info.addrResult);
	if (error != 0)
	{
		cout << "getaddrinfo error:" << error << endl;
		return -1;
	}

	info.sockfd = socket(info.addrResult->ai_family, info.addrResult->ai_socktype, info.addrResult->ai_protocol);
	if (info.sockfd == -1)
	{
		std::cout << "socket error:" << error << std::endl;
		return -1;
	}
	return 0;
}

void HttpDownloader::startDownload()
{
	for (unsigned int i = 0; i < mUrls.size(); i++)
	{
		startDownload(mUrls.at(i));
	}
}

int HttpDownloader::bindSSL(HttpDownloadUrlInfo & info)
{
	// New context saying we are a client, and using SSL 2 or 3
	info.sslContext = SSL_CTX_new (SSLv23_client_method ());
	if (info.sslContext == NULL)
	{
		ERR_print_errors_fp (stderr);
		return -1;
	}

	// Create an SSL struct for the connection
	info.sslHandle = SSL_new (info.sslContext);
	if (info.sslHandle == NULL)
	{
		ERR_print_errors_fp (stderr);
		return -1;
	}

	// Connect the SSL struct to our connection
	if (!SSL_set_fd (info.sslHandle, info.sockfd))
	{
		ERR_print_errors_fp (stderr);
		return -1;
	}
	return 0;
}

void * HttpDownloader::thread_recv_send(void* param)
{
	HttpDownloadUrlInfo * info = (HttpDownloadUrlInfo*)param;
	if (!info)
	{
		cout << "thread_recv_send get error param" << endl;
		return NULL;
	}

	FD_SET fdread;
	FD_SET fdwrite;

	timeval timout;
	timout.tv_sec = 3;
	timout.tv_usec = 0;

	//请求数据
	stringstream sendStream;
	sendStream << "GET " << info->getStr << " HTTP/1.1\r\n"
	            << "HOST: "<< info->host <<"\r\n"
	            << "connection: keep-alive\r\n\r\n";
	
	int totalSendByte = sendStream.str().size();
	int leftSendByte = totalSendByte;

	char writeBuff[WRITE_BUFF_SIZE];
	memset(writeBuff, 0, WRITE_BUFF_SIZE);
	memcpy(writeBuff, sendStream.str().c_str(), totalSendByte);

	//下载的文件
	FILE * file = fopen(info->saveFileName.c_str(), "wb");

	char readBuff[READ_BUFF_SIZE];

	//是否已经剥掉了http头
	bool hasStripedHead = false;

	while(true)
	{
		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);

		FD_SET(info->sockfd, &fdread);
		FD_SET(info->sockfd, &fdwrite);

		int err = select(info->sockfd+1, &fdread, &fdwrite, NULL, &timout);
		if (err == 0)
		{
			cout << "timeout" << endl;
			break;
		}
		else if (err > 0)
		{
			//正在连接状态, 如果需要，建立ssl链接
			if (info->status == HttpDownloadUrlInfo::E_CONNECTING && info->needSSL)
			{
				err = SSL_connect(info->sslHandle);
				if (err != 1)
				{
					int sslError = SSL_get_error(info->sslHandle, err);
					cout << "SSL_connect error:" << err << ", ssl error:" << sslError << endl;
					if (sslError == SSL_ERROR_WANT_READ || sslError == SSL_ERROR_WANT_WRITE)
					{
						continue;
					}
					break;
				}
			}

			//可写
			if (FD_ISSET(info->sockfd, &fdwrite) && leftSendByte > 0)
			{
				int writeIndex = totalSendByte - leftSendByte;

				cout << "select can write, total:" << totalSendByte << "left:" << leftSendByte << ", write buff:\n" << writeBuff << endl;

				int writeByte = SSL_write(info->sslHandle, writeBuff + writeIndex, leftSendByte);
				cout << "write " << writeByte << " bytes, left:" << leftSendByte << "bytes" << endl;

				if (writeByte <= 0)
				{
					int sslError = SSL_get_error(info->sslHandle, writeByte);
					cout << "SSL_write error:" << err << ", ssl error:" << sslError << endl;
					if (sslError == SSL_ERROR_WANT_READ || sslError == SSL_ERROR_WANT_WRITE)
					{
						continue;
					}
					else
					{
						break;
					}
				}
				
				leftSendByte -= writeByte;
			}
			else if (FD_ISSET(info->sockfd, &fdread))
			{
				cout << "select can read" << endl;

				memset(readBuff, 0, READ_BUFF_SIZE);
				int readByte = SSL_read(info->sslHandle, readBuff, READ_BUFF_SIZE);
				cout << "read " << readByte << " bytes, hasStripedHead:" << hasStripedHead << endl;

				if (readByte > 0)
				{
					int writeFileCount = 0;

					if (!hasStripedHead)
					{
						int index = HttpDownloader::stripHttpHeader(readBuff, readByte);
						if (0 < index && index < readByte)
						{
							hasStripedHead = TRUE;
							writeFileCount = fwrite(readBuff+index, 1, readByte-index, file);
							info->downloadSize += writeFileCount;
						}
						else
						{
							writeFileCount = fwrite(readBuff, 1, readByte, file);
							info->downloadSize += writeFileCount;
						}
					}
					else
					{
						writeFileCount = fwrite(readBuff, 1, readByte, file);
						info->downloadSize += writeFileCount;
					}
					cout << "download size:" << info->downloadSize << endl;
				}
				else if (readByte == 0)
				{
					cout << "read end" << endl;
					break;
				}
				else
				{
					int sslError = SSL_get_error(info->sslHandle, readByte);
					cout << "SSL_read error:" << err << ", ssl error:" << sslError << endl;
					if (sslError == SSL_ERROR_WANT_READ || sslError == SSL_ERROR_WANT_WRITE)
					{
						continue;
					}
					else
					{
						break;
					}
				}
			}
		}
		else
		{
			cout << "select error:" << WSAGetLastError() << endl;
			break;
		}
	}

	fclose(file);

	return NULL;
}

int HttpDownloader::startDownload(HttpDownloadUrlInfo & info)
{
	int err = createSocket(info);
	if (err != 0)
	{
		std::cout << "create socket error" << std::endl;
		return -1;
	}

	//非阻塞
	u_long mode = 1;
	err = ioctlsocket(info.sockfd, FIONBIO, &mode);
	if (err != NO_ERROR)
	{
		cout << "set non-block socket error" << endl;
		return -1;
	}

	//绑定到SSL
	if (info.needSSL)
	{
		err = bindSSL(info);
		if (err != 0)
		{
			std::cout << "bindSSL error" << std::endl;
			return -1;
		}
	}

	//连接
	err = connect(info.sockfd, info.addrResult->ai_addr, info.addrResult->ai_addrlen);
	cout <<"connect end, ret:" << err << endl;

	info.status = HttpDownloadUrlInfo::E_CONNECTING;

	//创建线程读写
	err = pthread_create(&info.pid, NULL, HttpDownloader::thread_recv_send, &info);
	if (err != 0)
	{
		info.status = HttpDownloadUrlInfo::E_NONE;
		cout << "create thread error" << endl;
		return -1;
	}
	return 0;
}

int test_interface(int argc, char **argv)
{
	WSADATA data;
	int err = WSAStartup(MAKEWORD(2, 2), &data);

	HttpDownloader downloader;
	downloader.initSSLEnv();

	//downloader.addUrl("https://yxsm.qq.com/activity/170104.jpg");
	//downloader.addUrl("https://yxsm.qq.com/activity/170106.jpg");
	//downloader.addUrl("https://yxsm.qq.com/activity/161213.jpg");
	//downloader.addUrl("https://yxsm.qq.com/activity/161206.jpg");
	downloader.addUrl("https://dlied5.qq.com/HeroGame/Release/BFile/20170213101302/all_table_server.zip");
	downloader.startDownload();

	vector<HttpDownloadUrlInfo> & infos = downloader.getUrls();
	for (u_int i = 0; i < infos.size(); i++)
	{
		if (infos.at(i).status != HttpDownloadUrlInfo::E_NONE)
		{
			void * status;
			pthread_join(infos.at(i).pid, &status);
		}
	}

	cout << "main thread end" << endl;
	return 0;
}

#endif