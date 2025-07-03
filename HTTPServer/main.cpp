#include <iostream>
#include "httpserver.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 3)
        return -1;
    HTTPServer serv(argv[1], argv[2]);
    serv.Run();
    return 0;
}
