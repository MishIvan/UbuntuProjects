#include "httpserver.h"

// Создание сокета и привязка его к серверу
HTTPServer::HTTPServer(const char * host, const char *port)
{
    m_host = host;
    m_port = port;

    addrinfo hints;
    addrinfo *result, *rp;
    int sfd, s;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    hints.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    char buff[64];

    s = getaddrinfo(m_host.c_str(), m_port.c_str(), &hints, &result);
    if (s != 0) {
        sprintf(buff, "getaddrinfo: %s\n", gai_strerror(s));
        throw buff;
    }

    /* getaddrinfo() returns a list of address structures.
              Try each address until we successfully bind(2).
              If socket(2) (or bind(2)) fails, we (close the socket
              and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */
        else
        {
            sprintf(buff,"%s", strerror(errno));
        }

        close(sfd);
    }

    freeaddrinfo(result);           /* No longer needed */

    if (rp == NULL)                /* No address succeeded */
        throw "Could not bind";
    else
    {
        m_socket = sfd;
    }
}

// Запуск цикла обработки запроса
void HTTPServer::Run()
{
    char buff[1024];
    listen(m_socket, 5);
    while(true)
    {
        sockaddr client;
        socklen_t addrlen = sizeof(client);
        int client_sock = accept(m_socket, &client,
                                 &addrlen);
        if(client_sock < 1)
        {
            throw "Could not accept receive client message";
            break;
        }
        int length;
        string msg;

        do
        {
            length = recv(client_sock, buff, 1024, MSG_DONTWAIT);
            if(length > 0) msg += buff;
        }while(length>0);

        string msg_resp;
        if(msg.length() > 0)
        {
            bool rp = Parse(msg); // парсить текст запроса
            msg_resp = rp ? "200 OK" : "400 - Error";
        }
        else
        {
            msg_resp = "400 - Error";
        }

        send(client_sock, msg_resp.c_str(), msg_resp.length(), 0);
        shutdown(client_sock,0);
        close(client_sock);

    }
}
// Парсить заголовок и тело запроса
bool HTTPServer::Parse(const string& request) {
    try {
        istringstream stream(request);
        string line;

        // Парсим первую строку
        getline(stream, line);
        istringstream firstLine(line);
        firstLine >> m_method >> m_path >> m_version;

        // Парсим заголовки
        while (getline(stream, line) && !line.empty()) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 2);
                m_headers[key] = value;
            }
        }
        if(m_method == "POST")
        {
            // Пропускаем пустую строку
            getline(stream, line);

            // Читаем тело запроса
            getline(stream, m_body, '\0');
        }
        return true;
    } catch (...) {
        return false;
    }
}

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
map<string, string> parsePostBody(const string& body) {
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

