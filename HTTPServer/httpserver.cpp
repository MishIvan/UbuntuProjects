#include "httpserver.h"

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
    string msg_resp;

    size_t pos = path.find('?');
    bool err = pos == string::npos;
    if(err)
    {
        msg_resp = "400 OK  = Bad Request";
        sprintf(buff, m_response_str, msg_resp.c_str());
        send(sock, msg_resp.c_str(), msg_resp.length(), 0);
        return;
    }

    string pear = path.substr(pos+1);
    map<string, string> params = parseParams(pear);
    path = "";
    for(auto el : params)
    {
        path += el.first+": "+el.second+"; ";
    }

    msg_resp = "200 OK";
    sprintf(buff, m_response_str, msg_resp.c_str());
    msg_resp = buff;
    msg_resp +="\r\n\r\nParameters are:"+path;
    send(sock, msg_resp.c_str(), msg_resp.length(), 0);

}
void HTTPServer::do_POST(const RequestData& recvdata)
{
    int sock = recvdata.m_socket;
    string path = recvdata.m_path;
    char buff[128];

    string msg_resp = "200 OK";
    sprintf(buff, m_response_str, msg_resp.c_str());
    msg_resp = buff;
    msg_resp +"\r\n\r\n"+path;
    send(sock, msg_resp.c_str(), msg_resp.length(), 0);

}
