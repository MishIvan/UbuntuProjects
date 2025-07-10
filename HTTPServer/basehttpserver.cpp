#include "basehttpserver.h"
#include "reply.h"

// Создание сокета и привязка его к серверу
BaseHTTPServer::BaseHTTPServer(const char * host, const char *port)
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
void BaseHTTPServer::Run()
{    
    listen(m_socket, 5);
    // функция обработки запроса
    auto fn =  [&]()
    {
        char buff[BUFF_SIZE];
        sockaddr client;
        socklen_t addrlen = sizeof(client);
        int client_sock = accept(m_socket, &client,
                                 &addrlen);
        if(client_sock < 1)
        {
            throw "Could not accept receive client message";
            return;
        }

        ssize_t bytes_read = 0;
        string msg;
        do
        {
            bytes_read = read(client_sock, buff, BUFF_SIZE);
            if(bytes_read > 0)
            {
                msg += buff;
                if(bytes_read <= BUFF_SIZE) break;
            }
        }while(bytes_read>0);

        RequestData rdata;
        Reply repl;
        rdata.m_socket = client_sock;
        if(msg.length() > 0)
        {
            bool rp = Parse(msg, rdata); // парсить текст запроса
            if(!rp) // не удалось спарсить текст запроса
            {
                repl.setStatus(400);
                repl.SetHeader("Content-Type","text/html; charset=UTF-8");
                string body = "Ошибка при разборе текста запроса";
                repl.setBody(body.c_str());
                sprintf(buff,"%d", (int)body.length());
                repl.SetHeader("Content-Length",buff);
                repl.Send(client_sock);

                shutdown(client_sock,0);
                close(client_sock);
                return;

            }
            if(rdata.m_method == "POST")
                do_POST(rdata);
            else if(rdata.m_method == "GET")
                do_GET(rdata);
            else
            {
                repl.setStatus(405);
                repl.SetHeader("Content-Type","text/html; charset=UTF-8");
                string body = "Метод не поддерживается";
                repl.setBody(body.c_str());
                sprintf(buff,"%d", (int)body.length());
                repl.SetHeader("Content-Length",buff);
                repl.Send(client_sock);

                shutdown(client_sock,0);
                close(client_sock);
                return;


            }


        }
        else // данные запроса не получены
        {
            repl.setStatus(404);
            repl.SetHeader("Content-Type","text/html; charset=UTF-8");
            string body = "Данные от клиента не получены";
            repl.setBody(body.c_str());
            sprintf(buff,"%d", (int)body.length());
            repl.SetHeader("Content-Length",buff);
            repl.Send(client_sock);

        }

        shutdown(client_sock,0);
        close(client_sock);
    };

    while(true)
    {
        packaged_task<void()> task(fn);
        auto future = task.get_future();
        thread th(move(task));
        th.detach();
        future.get();
    }
}
// Парсить заголовок и тело запроса
bool BaseHTTPServer::Parse(const string& request,
                       RequestData& recv_data) {
    try {
        istringstream stream(request);
        string line;

        // Парсим первую строку
        getline(stream, line);
        istringstream firstLine(line);
        firstLine >> recv_data.m_method >> recv_data.m_path >> recv_data.m_version;

        // Парсим заголовки
        while (getline(stream, line) && !line.empty()) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 2);
                recv_data.m_headers[key] = value;
            }
        }
        if(recv_data.m_method == "POST")
        {
            // Пропускаем пустую строку
            getline(stream, line);

            // Читаем тело запроса
            getline(stream, recv_data.m_body, '\0');
        }
        return true;
    } catch (...) {
        return false;
    }
}


