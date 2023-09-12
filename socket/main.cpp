#include "Request.hpp"
#include "Webserv.hpp"

int main()
{
    Webserv ws;
    if (ws.init() == -1)
        return 0;
    if (ws.process() == -1)
        return 0;
}