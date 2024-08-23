#include <stdio.h>
#include <string.h>
#include "httpCurlUtils.h"

size_t httpCurlUtils::readData(void *ptr, size_t size, size_t nmemb, void *stream)
{
	string *d = (string *)stream;
	int len = size * nmemb;
	if (len > d->length())
		len = d->length();

	memcpy(ptr, d->c_str(), len);
	string tmpStr = d->substr(len);
	*d = tmpStr;

	return size * nmemb;
}

size_t httpCurlUtils::writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
	string *d = (string *)stream;
	d->append((const char *)ptr, size*nmemb);

	return size*nmemb;
}

string httpCurlUtils::httpEasyPostImage(const char *url, vector<string> header, const char *file, bool hasTable, int *pRet)
{
	string respStr;

	struct curl_slist* headers = NULL;

	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;

	CURL *easy_handle = curl_easy_init();

	if (easy_handle){
		if (strncmp(url, "https", 5) == 0){
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0);
		}

		curl_easy_setopt(easy_handle, CURLOPT_URL, url);

		for (size_t i = 0; i < header.size(); i++)
			headers = curl_slist_append(headers, header[i].c_str());
		curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
		

		curl_formadd(&post, &last, CURLFORM_PTRNAME, "file", CURLFORM_FILE, file, CURLFORM_END);
		curl_formadd(&post, &last, CURLFORM_PTRNAME, "hasTable", CURLFORM_PTRCONTENTS, hasTable ? "true" : "false", CURLFORM_END);
		//curl_formadd(&post, &last, CURLFORM_PTRNAME, "direction", CURLFORM_PTRCONTENTS, "0", CURLFORM_END);

		curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, post);

		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writeData);
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &respStr);
		curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, mTimeout * 1000);//ms


		CURLcode res = curl_easy_perform(easy_handle);
		if (res != CURLE_OK){
			printf("curl_easy_perform failed: %s\r\n", curl_easy_strerror(res));
		}

		if (pRet) *pRet = res;

		curl_easy_cleanup(easy_handle);
	}

	return respStr;
}

string httpCurlUtils::httpEasyPostImage(const char *url, vector<string> header, const char *file, const char* type, int *pRet)
{
	string respStr;
    printf("url:%s, type:%s, file:%s", url, type, file);
	struct curl_slist* headers = NULL;

	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;

	CURL *easy_handle = curl_easy_init();

	if (easy_handle){
		if (strncmp(url, "https", 5) == 0){
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0);
		}

		curl_easy_setopt(easy_handle, CURLOPT_URL, url);

		for (size_t i = 0; i < header.size(); i++)
			headers = curl_slist_append(headers, header[i].c_str());
		curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
		
		curl_formadd(&post, &last, CURLFORM_PTRNAME, "file", CURLFORM_FILE, file, CURLFORM_END);
		curl_formadd(&post, &last, CURLFORM_PTRNAME, type, CURLFORM_PTRCONTENTS, "true", CURLFORM_END);
		//curl_formadd(&post, &last, CURLFORM_PTRNAME, "direction", CURLFORM_PTRCONTENTS, "0", CURLFORM_END);

		curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, post);

		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writeData);
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &respStr);
		curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, mTimeout * 1000);//ms


		CURLcode res = curl_easy_perform(easy_handle);
		if (res != CURLE_OK){
			printf("curl_easy_perform failed: %s\r\n", curl_easy_strerror(res));
		}

		if (pRet) *pRet = res;

		curl_easy_cleanup(easy_handle);
	}

	return respStr;
}


string httpCurlUtils::httpEasyPostImage(const char *url, vector<string> header, const char *file, const char* type, bool isLocalFile, int *pRet)
{
	string respStr;
    printf("url:%s, type:%s, file:%s", url, type, file);
	struct curl_slist* headers = NULL;

	struct curl_httppost* post = NULL;
	struct curl_httppost* last = NULL;

	CURL *easy_handle = curl_easy_init();

	if (easy_handle){
		if (strncmp(url, "https", 5) == 0){
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0);
		}

		curl_easy_setopt(easy_handle, CURLOPT_URL, url);

		for (size_t i = 0; i < header.size(); i++)
			headers = curl_slist_append(headers, header[i].c_str());
		curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
		if(isLocalFile)
			curl_formadd(&post, &last, CURLFORM_PTRNAME, "localfile", CURLFORM_PTRCONTENTS, file, CURLFORM_END);
		else
			curl_formadd(&post, &last, CURLFORM_PTRNAME, "file", CURLFORM_FILE, file, CURLFORM_END);
		curl_formadd(&post, &last, CURLFORM_PTRNAME, type, CURLFORM_PTRCONTENTS, "true", CURLFORM_END);
		//curl_formadd(&post, &last, CURLFORM_PTRNAME, "direction", CURLFORM_PTRCONTENTS, "0", CURLFORM_END);

		curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, post);

		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writeData);
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &respStr);
		curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, mTimeout * 1000);//ms


		CURLcode res = curl_easy_perform(easy_handle);
		if (res != CURLE_OK){
			printf("curl_easy_perform failed: %s\r\n", curl_easy_strerror(res));
		}

		if (pRet) *pRet = res;

		curl_easy_cleanup(easy_handle);
	}

	return respStr;
}

string httpCurlUtils::httpEasyPost(const char *url, const char *data, int size, const char *extraHeader, int *pRet)
{
	string respStr;

	struct curl_slist* headers = NULL;

	CURL *easy_handle = curl_easy_init();

	if (easy_handle){
		if (strncmp(url, "https", 5) == 0){
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0);
		}

		curl_easy_setopt(easy_handle, CURLOPT_URL, url);

		//ԭʼbody
		curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, data);

		if (extraHeader)
			headers = curl_slist_append(headers, extraHeader);
		headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writeData);
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &respStr);
		curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, mTimeout * 1000);//ms

		curl_easy_setopt(easy_handle, CURLOPT_POST, 1);

		CURLcode res = curl_easy_perform(easy_handle);
		if (res != CURLE_OK){
			printf("curl_easy_perform failed: %s\r\n", curl_easy_strerror(res));
		}

		if (pRet) *pRet = res;

		curl_easy_cleanup(easy_handle);
	}

	return respStr;
}

string httpCurlUtils::httpEasyPut(const char *url, const char *data, int size, const char *extraHeader)
{
	string respStr;
	string reqStr;
	reqStr.assign(data, size);

	struct curl_slist* headers = NULL;

	CURL *easy_handle = curl_easy_init();

	if (easy_handle){
		if (strncmp(url, "https", 5) == 0){
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0);
		}

		curl_easy_setopt(easy_handle, CURLOPT_URL, url);

		if (extraHeader)
			headers = curl_slist_append(headers, extraHeader);
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Cache-Control: no-cache");
		headers = curl_slist_append(headers, "Connection: close");
		curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(easy_handle, CURLOPT_PUT, 1L);
		curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, readData);
		curl_easy_setopt(easy_handle, CURLOPT_READDATA, &reqStr);
		curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE, reqStr.length());

		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writeData);
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &respStr);
		curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, mTimeout * 1000);//ms

		CURLcode res = curl_easy_perform(easy_handle);
		if (res != CURLE_OK){
			printf("curl_easy_perform failed: %s\r\n", curl_easy_strerror(res));
		}

		curl_easy_cleanup(easy_handle);
	}

	return respStr;
}

string httpCurlUtils::httpEasyPostWithHeader(string url, string data, vector<string> header, int *pRet)
{
	string respStr;

	struct curl_slist* headers = NULL;

	CURL *easy_handle = curl_easy_init();

	if (easy_handle){
		if (url.substr(0, 5) == "https"){
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0);
		}

		curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());

		//原始body
		if (data.empty())
			curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, NULL);
		else
			curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, data.c_str());

		for (size_t i = 0; i < header.size(); i++)
			headers = curl_slist_append(headers, header[i].c_str());
		curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writeData);
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &respStr);
		curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1);//多线程支持
		curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, mTimeout * 1000);//ms

		curl_easy_setopt(easy_handle, CURLOPT_POST, 1);

		CURLcode res = curl_easy_perform(easy_handle);
		if (res != CURLE_OK){
			printf("curl_easy_perform 失败返回: %s\r\n", curl_easy_strerror(res));
		}

		if (pRet) *pRet = res;

		curl_easy_cleanup(easy_handle);
	}

	return respStr;
}

string httpCurlUtils::urlEncode(const string &src)
{
	string dst;

	CURL *easy_handle = curl_easy_init();
	if (easy_handle) {
		char *output = curl_easy_escape(easy_handle, src.c_str(), src.length());
		if (output) {
			dst = output;
			curl_free(output);
		}
		curl_easy_cleanup(easy_handle);
	}

	return dst;
}

bool httpCurlUtils::httpConnectTest(string url)
{
	int httpRet;
	httpCurlUtils http(3);

	http.httpEasyPost(url.c_str(), "T", 1, NULL, &httpRet);

	return httpRet == 0;
}