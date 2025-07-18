#ifndef REPLY_H
#define REPLY_H

#include <map>
#include <string>
#include <sys/socket.h>

using namespace std;

class ReplyOrRequest
{
protected:
    map<string, string> m_headers;
    string m_body;
public:
    virtual void Send(int socket) = 0;
    void SetHeader(const char *key, const char* value);
    void setBody(const char *body) {m_body = body;}

};

class Reply : public ReplyOrRequest
{
    int m_status;
public:
    Reply() {}
    void Send(int socket);
    void setStatus(int st) {m_status = st;}
};

class Request : public ReplyOrRequest
{
    string m_method;
    string m_host;
    string m_path;
public:
    Request() {}
    void Send(int socket);
    void setHost(string value) { m_host = value;}
    void setMethod(string value) { m_method = value;}
    void setPath(string value) { m_path = value;}
};

#endif // REPLY_H
