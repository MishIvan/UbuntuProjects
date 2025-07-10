#include <iostream>
#include "httpserver.h"

using namespace std;

int _daemon() {
    pid_t pid;
    pid = fork(); // Fork off the parent process
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        cout << "Usage: HTTPServer host port\n";
        return -1;
    }
    _daemon();
    HTTPServer serv(argv[1], argv[2]);
    serv.Run();
    return 0;
}
