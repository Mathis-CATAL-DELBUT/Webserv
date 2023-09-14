#ifndef RESPONSE_H
# define RESPONSE_H

#include "Request.hpp"
#include "Webserv.hpp"

class Request ;

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
        void addingFormat(Request &request);
        void getContent();
        void getPathToContent(Request &request);
};

#endif