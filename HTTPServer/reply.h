#ifndef REPLY_H
#define REPLY_H

#include <map>
#include <string>
#include <sys/socket.h>

using namespace std;

class Reply
{
    map<string, string> m_headers;
    int m_status;
    string m_body;
public:
    Reply();
    void Send(int socket);
    void SetHeader(const char *key, const char* value);
    void setStatus(int st) {m_status = st;}
    void setBody(const char *body) {m_body = body;}
};

#endif // REPLY_H
