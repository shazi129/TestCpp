#include "./../TestInterface.h"


#ifdef HttpsConnection

#include <WINSOCK2.H>
#include <WS2tcpip.h>
#include <iostream>
#include <sstream>

#include "pthread.h"
#include "./HttpsConnection.h"

#define SAVENAME "./161229.jpg"

using namespace std;


int HttpDownloader::stripHttpHeader(char * buff, int buffSize)
{
	for (int i = 0; i < buffSize-4; i++)
	{
		if (buff[i] != 0x0D
			&& buff[i+1] != 0x0A
			&& buff[i+2] != 0x0D
			&& buff[i+3] != 0x0A)
		{
			return i+4;
		}
		return i+4;
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
		mUrls.at(i).status = 0;
		int err = startDownload(mUrls.at(i));
		if (err == 0)
		{
			mUrls.at(i).status = 1;
		}
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

	//创建线程读写
	err = pthread_create(&info.pid, NULL, HttpDownloader::thread_recv_send, &info);
	if (err != 0)
	{
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

	downloader.addUrl("https://yxsm.qq.com/activity/161229.jpg");
	downloader.startDownload();

	vector<HttpDownloadUrlInfo> & infos = downloader.getUrls();
	for (u_int i = 0; i < infos.size(); i++)
	{
		if (infos.at(i).status != 0)
		{
			void * status;
			pthread_join(infos.at(i).pid, &status);
		}
	}

	cout << "main thread end" << endl;

	/*
	// Initiate SSL handshake
	while (1)
	{
		int ret = SSL_connect (sslHandle);
		if (ret != 1)
		{
			int error = SSL_get_error(sslHandle, ret);
			if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
			{
				std::cout << "SSLConnect get WANT IO ERROR" << std::endl;
				continue;
			}
			std::cout << "SSLConnect error" << error << std::endl;
			return 0;
		}
		break;
	}
	

	std::stringstream ss;
	ss << "GET " << GET << " HTTP/1.1\r\n"<<"HOST: "<<HOST<<"\r\n"<<"connection: keep-alive\r\n\r\n";
	std::cout << ss.str().c_str();

	while(1)
	{
		int ret = SSL_write (sslHandle, (char*)ss.str().c_str(), strlen (ss.str().c_str()));
		if (ret < 0)
		{
			int error = SSL_get_error(sslHandle, ret);
			if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
			{
				std::cout << "SSL Write get WANT IO ERROR" << std::endl;
				continue;
			}
			std::cout << "SSL Write error" << error << std::endl;
			return 0;
		}
		break;
	}
	

	const int readSize = 102400;
	char buffer[102400];

	FILE * file = fopen("./a.jpg", "wb");

	bool hasStripedHead = FALSE;

	while (1)
	{
		memset(buffer, 0, readSize);
		int received = SSL_read (sslHandle, buffer, readSize - 1);
		if (received < 0)
		{
			int error = SSL_get_error(sslHandle, received);
			if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
			{
				std::cout << "SSLRead get WANT IO ERROR" << std::endl;
				continue;
			}
			std::cout << "SSLRead error:" << error << std::endl;
			return 0;
		}
		else if (received > 0)
		{
			printf ("%s", buffer);
			int writeCount = 0;
			if (!hasStripedHead)
			{
				int index = HttpDownloader::stripHttpHeader(buffer, received);
				if (0 < index && index < received)
				{
					hasStripedHead = TRUE;
					writeCount = fwrite(buffer+index, 1, received-index, file);
				}
				else
				{
					writeCount = fwrite(buffer, 1, received, file);
				}
			}
			else
			{
				writeCount = fwrite(buffer, 1, received, file);
			}
			continue;
		}
		fflush(file);
		
		fclose(file);
		std::cout << "SSLRead end" << std::endl;

		break;
	}

	if (socket)
		closesocket(socket);
	if (sslHandle)
	{
		SSL_shutdown (sslHandle);
		SSL_free (sslHandle);
	}
	if (sslContext)
		SSL_CTX_free (sslContext);
	*/
	return 0;
}

#endif