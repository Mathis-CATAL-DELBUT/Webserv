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

        void requestTreatment(char *request);
        // void print();

        void setMethod(int a);
        void setStatusCode(int a);
        void setProtocol(std::string str);
        void setRessourcePath(std::string str);
        void setContentType(std::string str);
        void setBufferRequest(char *str);

        int getMethod() const;
        int getStatusCode() const;
        std::string getProtocol() const;
        std::string getRessourcePath() const;
        std::string getContentType() const;


    private:
        int _method;
        int _statusCode;
        std::string _protocol;
        std::string _ressourcePath;
        std::string _contentType;
        char bufferRequest[BUFFER_SIZE];

        // void parsingFormat();
        // void statutLineParsing();
        // void headerParsing();
        // void bodyParsing();
};

#endif