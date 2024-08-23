#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "plog/Log.h"
#include "plog/Appenders/ColorConsoleAppender.h"
#include "plog/Appenders/RollingFileAppender.h"
#include "httpCurlUtils.h"
#include "httplib.h"
#include "EvServer.h"

using namespace std;

void httpServer()
{
    // httplib::Server svr;
    // svr.Post("/hi", [](const httplib::Request& req, httplib::Response& res) {
    //     res.set_content("Hello, World!", "text/plain");
    // });
    // svr.listen("localhost", 8080);
    EvHttpsServerInit(8080);
    return;
}

int main()
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    PLOGI << "Hello, Log!";
    httpCurlUtils curl;
    
    thread server_thread(httpServer);
    server_thread.detach();

    sleep(1);
    string req = "0123456789";
    string str = curl.httpEasyPost("https://localhost:8080/hi", req.c_str(), req.size(), "");
    PLOGI << "httpEasyPost result:" << str;

    
    while(1);
    return 0;
}