#pragma once
#include "curl/curl.h"
#include <string>
#include <vector>

using namespace std;

class httpCurlUtils{
public:
	httpCurlUtils(int timeout = 30){
		mTimeout = timeout;
	}
	int mRetCode;
	int mTimeout;//seconds

	string httpEasyPost(const char *url, const char *data, int size, const char *extraHeader, int *pRet = NULL);//form-urlencoded; utf-8
	string httpEasyPut(const char *url, const char *data, int size, const char *extraHeader);
	string httpEasyPostWithHeader(string url, string data, vector<string> header, int *pRet = NULL);
	string httpEasyPostImage(const char *url, vector<string> header, const char *file, bool hasTable = true, int *pRet = NULL);
    string httpEasyPostImage(const char *url, vector<string> header, const char *file, const char* type, int *pRet = nullptr);
    string httpEasyPostImage(const char *url, vector<string> header, const char *file, const char* type, bool isLocalFile, int *pRet = nullptr);
	string urlEncode(const string &src);

	static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t readData(void *ptr, size_t size, size_t nmemb, void *stream);
	static bool httpConnectTest(string url);
};