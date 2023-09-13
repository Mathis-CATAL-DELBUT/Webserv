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
#include <algorithm>
#include <vector>

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

        // void setMethod(int a);
        // void setStatusCode(int a);
        // void setProtocol(std::string str);
        // void setRessourcePath(std::string str);
        // void setContentType(std::string str);
        void setRawRequest(std::string str);

        int getMethod() const;
        int getStatusCode() const;
        std::string getProtocol() const;
        std::string getRessource() const;
        std::string getContentType() const;
        std::string getRawRequest() const;
        std::string getExtension() const;

    private:
        int _method;
        int _statusCode;
        std::string _protocol;
        std::string _ressource;
        std::string _extension;
        std::string _contentType;
        std::string rawRequest;

        void parsingFormat();
        std::string infile(std::string str, char c);
        // void statutLineParsing();
        // void headerParsing();
        // void bodyParsing();
};

#endif