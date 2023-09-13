#include "Request.hpp"
#include "Webserv.hpp"

int main()
{
    Webserv ws;
    if (ws.init() == false)
        return 0;
    if (ws.process() == false)
        return 0;
}