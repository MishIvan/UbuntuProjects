#include "httpserver.h"
#include "reply.h"
#include "../Common/json.hpp"

using json = nlohmann::json;


// Функция декодирования URL
string urlDecode(const string& encoded) {
    string decoded;
    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 < encoded.length()) {
                string hex = encoded.substr(i + 1, 2);
                char ch = static_cast<char>(stoi(hex, nullptr, 16));
                decoded += ch;
                i += 2;
            }
        } else if (encoded[i] == '+') {
            decoded += ' ';
        } else {
            decoded += encoded[i];
        }
    }
    return decoded;
}

// Функция для парсинга параметров из тела запроса
map<string, string> parseParams(const string& body) {
    map<string, string> params;
    string decodedBody = urlDecode(body);

    // Разделяем параметры
    regex param_regex("([^&=]+)=([^&]*)");
    auto begin = sregex_iterator(decodedBody.begin(),
                                 decodedBody.end(), param_regex);
    auto end = sregex_iterator();

    for (sregex_iterator i = begin; i != end; ++i) {
        smatch match = *i;
        params[match.str(1)] = match.str(2);
    }

    return params;
}

HTTPServer::HTTPServer(const char * host, const char *port)
    :BaseHTTPServer(host, port) {}

void HTTPServer::do_GET(const RequestData& recvdata)
{
    int sock = recvdata.m_socket;
    string path = recvdata.m_path;
    char buff[128];

    size_t pos = path.find('?');

    bool err = pos == string::npos;
    Reply repl;
    json j;
    if(err)
    {        
        repl.setStatus(400);
        repl.SetHeader("Content-Type","application/json");

        j["status"] = 400;
        j["errorMessage"] = "Неверное задание пути в запросе";

        string body = j.dump();
        sprintf(buff,"%d",(int)body.length());
        repl.SetHeader("Content-Length", buff);
        repl.setBody(body.c_str());
        repl.Send(sock);
        return;
    }

    string pear = path.substr(pos+1);
    map<string, string> params = parseParams(pear);
    j["status"] = 200;
    j["errorMessage"] = nullptr;
    for(auto el : params)
    {
        j[el.first.c_str()] = el.second.c_str();
    }

    //sleep(3);
    repl.setStatus(200);
    repl.SetHeader("Content-Type","application/json");

    string body = j.dump();
    repl.setBody(body.c_str());
    sprintf(buff,"%d",(int)body.length());
    repl.SetHeader("Content-Length", buff);
    repl.Send(sock);

}
void HTTPServer::do_POST(const RequestData& recvdata)
{
    string path = recvdata.m_path;
    char buff[128];

    Reply repl;
    repl.setStatus(200);
    repl.SetHeader("Content-Type","text/html; charset=UTF-8");
    string body = path;
    repl.setBody(body.c_str());
    sprintf(buff,"%d", (int)body.length());
    repl.SetHeader("Content-Length",buff);
    repl.Send(recvdata.m_socket);

}
