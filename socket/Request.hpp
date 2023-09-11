#ifndef REQUEST_H
# define REQUEST_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <fstream>

# define BUFFER_SIZE 10000
# define GET 0
# define POST 1
# define DELETE 2

class Request
{
    public:
        Request();
        Request(Request const &copy);
        ~Request();
        Request &operator=(Request const &rhs);

        void parsingFormat();

    private:
        int _method;
        int _statusCode;
        int _returnCode;
        std::string _protocol;
        std::string _ressourcePath;
        std::string _contentType;
        char bufferRequest[BUFFER_SIZE];

        void statutLineParsing();
        void headerParsing();
        void bodyParsing();
};

#endif