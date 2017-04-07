#ifndef _HTTPS_CONNECTION_H_
#define _HTTPS_CONNECTION_H_

#include "openssl/ssl.h"
#include "openssl/rand.h"
#include "openssl/err.h"
#include "openssl/conf.h"
#include "openssl/engine.h"

#include "pthread.h"

#include <iostream>
#include <vector>

class HttpDownloadUrlInfo
{
public:
	enum E_DOWNLOAD_STATUS
	{
		E_NONE,
		E_CONNECTING,
		E_DOWNLOADING,
		E_DOWNLOADED,
	};

public:
	std::string host;
	int port;
	std::string getStr;
	std::string saveFileName;

	int sockfd;
	struct addrinfo*  addrResult;

	bool needSSL;
	SSL * sslHandle;
	SSL_CTX * sslContext;

	pthread_t pid; //当前下载的线程id
	E_DOWNLOAD_STATUS status; //0表示

	int downloadSize;

public:
	HttpDownloadUrlInfo()
	{
		status = E_NONE;
		sslHandle = NULL;
		sslContext = NULL;
		addrResult = NULL;
		sockfd = 0;
		downloadSize = 0;
	}
};

class HttpDownloader
{

public:
	HttpDownloader();
	~HttpDownloader();

	void addUrl(const std::string & url);
	void startDownload();
	
	int startDownload(HttpDownloadUrlInfo & info);

public:
	static int initUrlInfoWithUrl(const std::string & url, HttpDownloadUrlInfo & info);
	static int stripHttpHeader(char * buff, int buffSize);
	static void * thread_recv_send(void* param);

	void initSSLEnv();
	void destroySSLEnv();

	int createSocket(HttpDownloadUrlInfo & info);
	int bindSSL(HttpDownloadUrlInfo & info);

    std::vector<HttpDownloadUrlInfo> & getUrls();

	//创建一个线程首发消息
	void createNetThread();

private:
	std::vector<HttpDownloadUrlInfo> mUrls;

};

#endif