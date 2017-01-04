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
	std::string host;
	int port;
	std::string getStr;

	int sockfd;
	struct addrinfo*  addrResult;

	bool needSSL;
	SSL * sslHandle;
	SSL_CTX * sslContext;

	pthread_t pid; //��ǰ���ص��߳�id
	int status;
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

	//����һ���߳��׷���Ϣ
	void createNetThread();

private:
	std::vector<HttpDownloadUrlInfo> mUrls;

};

#endif