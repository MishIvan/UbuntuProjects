#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <string>
#include <cstring>
#include <map>
#include <sstream>
#include <iostream>
#include <regex>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "unistd.h"

using namespace std;

class HTTPServer
{
private:
    string m_host;
    string m_port;
    int m_socket;
    string m_method;
    string m_path;
    string m_version;
    map<string, string> m_headers;
    string m_body;
    bool Parse(const string& request);
public:
    HTTPServer(const char * host, const char *port);
    void Run();    
    map<string, string> getHeaders() const { return m_headers; }
    string getBody() const { return m_body; }
    string getMethod() const { return m_method; }
    string getPath() const { return m_path; }
    string getVersion() const { return m_version; }
};

#endif // HTTPSERVER_H
