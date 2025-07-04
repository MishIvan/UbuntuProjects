#ifndef BASEHTTPSERVER_H
#define BASEHTTPSERVER_H
#include <string>
#include <cstring>
#include <map>
#include <sstream>
#include <iostream>
#include <regex>
#include <future>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "unistd.h"

using namespace std;
#define BUFF_SIZE 1024
struct RequestData
{
    int m_socket;
    string m_method;
    string m_path;
    string m_version;
    map<string, string> m_headers;
    string m_body;
};

class BaseHTTPServer
{
protected:
    const char* m_response_str = "HTTP/1.1 %s\r\nContent-Type: text/html; charset=UTF-8";
    string m_host;
    string m_port;
    int m_socket;
    bool Parse(const string& request, RequestData& recv_data);
public:
    BaseHTTPServer(const char * host, const char *port);
    virtual void Run();
    virtual void do_GET(const RequestData& recvdata) {}
    virtual void do_POST(const RequestData& recvdata) {}
};

#endif // BASEHTTPSERVER_H
