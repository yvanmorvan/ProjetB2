#include <iostream>
#include "include/Server.h"
#include "include/winsock2.h"

#pragma comment(lib, "../lib/libws2_32.a")

using namespace std;

int main()
{
    Server server(8080);

    server.init();
    server.start();
    return 0;
}
