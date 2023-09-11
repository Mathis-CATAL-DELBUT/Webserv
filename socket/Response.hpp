#ifndef RESPONSE_H
# define RESPONSE_H

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

class Response
{
    public:
        Response();
        Response(Response const &copy);
        ~Response();
        Response &operator=(Response const &rhs);

        void addingFormat();

    private:
        int _statusCode;
        int _contentLength;
        int _returnCode;
        std::string _protocol;
        std::string _statusText;
        std::string _contentType;
        std::string _response;
        char bufferResponse[BUFFER_SIZE];

        void statutLineFormating();
        void headerFormating();
        void bodyFormating();
};

#endif