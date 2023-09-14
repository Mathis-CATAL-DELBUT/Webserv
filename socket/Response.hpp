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
#include <algorithm>
#include <vector>
#include <unordered_map>

#include "Request.hpp"

class Response
{
    public:
        Response();
        Response(Response const &copy);
        ~Response();
        Response &operator=(Response const &rhs);

        void generateResponse(Request &request);

        int getContentLength() const;
        std::string getResponse() const;


    private:
        int _statusCode;
        int _contentLength;
        std::string _pathToContent;
        std::string _rawContent;
        std::string _statusText;
        std::string _contentType;
        std::string _response;
        std::string _protocol;
        std::string _modifExtension;

        void addingFormat(Request &request);
        void configHeader(Request &request);
        void configBody(Request &request);
        void buildingFormattedResponse();
        void getContent();
        void getPathToContent(Request &request);
};

#endif