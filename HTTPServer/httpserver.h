#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "basehttpserver.h"

class HTTPServer : public BaseHTTPServer
{
public:
    HTTPServer(const char * host, const char *port);
    void do_GET(const RequestData& recvdata);
    void do_POST(const RequestData& recvdata);

};

#endif // HTTPSERVER_H
