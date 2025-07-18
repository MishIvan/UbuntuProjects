#include "reply.h"

void Reply::Send(int socket)
{
    string reply = "HTTP/1.1 ";
    switch(m_status)
    {
        case 200:
            reply += "200 OK\r\n"; break;
        case 400:
            reply += "400 Bad Request\r\n"; break;
        case 401:
            reply += "401 Unautorized\r\n"; break;
        case 404:
            reply += "404 Not Found\r\n"; break;
        case 403:
            reply += "403 Forbidden\r\n"; break;
        case 405:
            reply +="405 Method Not Allowed\r\n";break;
        case 406:
            reply += "406 Not Acceptable\r\n"; break;
        case 408:
            reply += "408 Request Timeout\r\n"; break;
        case 411:
            reply += "411 Length Required\r\n"; break;
        case 414:
            reply += "414 Request URI Too Long\r\n"; break;
        case 415:
            reply += "415 Unsupported Media Tipe\r\n"; break;
    }

    for(auto el : m_headers)
    {
        reply+= el.first +": "+el.second+"\r\n";
    }
    reply+="\r\n"+m_body;
    send(socket, reply.c_str(), reply.length(), 0);
}

void Request::Send(int socket)
{
    string reply = m_method + ' ' + m_path + " HTTP/1.1\r\n";

    for(auto el : m_headers)
    {
        reply+= el.first +": "+el.second+"\r\n";
    }
    reply+="\r\n"+m_body;
    send(socket, reply.c_str(), reply.length(), 0);
}

void ReplyOrRequest::SetHeader(const char *key, const char *value)
{
    string first = key;
    string second = value;
    m_headers[first] = second;
}
