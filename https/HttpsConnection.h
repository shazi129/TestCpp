#ifndef _HTTPS_CONNECTION_H_
#define _HTTPS_CONNECTION_H_

#include "openssl/ssl.h"
#include "openssl/rand.h"
#include "openssl/err.h"
#include "openssl/conf.h"
#include "openssl/engine.h"

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
};

class HttpDownloader
{

public:
	HttpDownloader();
	~HttpDownloader();

	void addUrl(const std::string & url);
	void startDownLoad();

public:
	static int initUrlInfoWithUrl(const std::string & url, HttpDownloadUrlInfo & info);
	static int stripHttpHeader(char * buff, int buffSize);

	void initSSLEnv();
	void destroySSLEnv();

	int createSocket(HttpDownloadUrlInfo & info);

    std::vector<HttpDownloadUrlInfo> & getUrls();

private:
	std::vector<HttpDownloadUrlInfo> mUrls;

};

#endif