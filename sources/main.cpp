#include "Parsing.hpp"
#include "Webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"


int main(int argc, char **argv)
{
    if (argc != 2)
        return (std::cout << "Error: number of arguments" << std::endl, 1);
    // signal(SIGINT, SIG_IGN);
    
    Parsing* config = new Parsing(argv[1]);

    Webserv ws(config);
    if (ws.initAllServ() == false)
        return delete config, 1;
    if (ws.processAllServ() == false)
        return delete config, 1;
    return (delete config, 0);
}