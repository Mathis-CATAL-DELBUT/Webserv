#include "Request.hpp"

Request::Request() {}

Request::~Request() {}

Request::Request(Request const &copy) {}

Request &Request::operator=(Request const &rhs)
{
    if (this == &rhs)
        return (*this);
    return (*this);
}

void Request::setMethod(int a)
{
    this->_method = a;
}
void Request::setStatusCode(int a)
{
    this->_statusCode = a;
}
void Request::setProtocol(std::string str)
{
    this->_protocol = str;
}
void Request::setRessourcePath(std::string str)
{
    this->_ressourcePath = str;
}
void Request::setContentType(std::string str)
{
    this->_contentType = str;
}
void Request::setBufferRequest(char *str)
{
    memcpy(this->bufferRequest, str, strlen(str));
}

int Request::getMethod() const
{
    return (this->_method);
}

int Request::getStatusCode() const
{
    return (this->_statusCode);
}

std::string Request::getProtocol() const
{
    return (this->_protocol);
}

std::string Request::getRessourcePath() const
{
    return (this->_ressourcePath);
}

std::string Request::getContentType() const
{
    return (this->_contentType);
}

void Request::requestTreatment(char *buffer)
{
    std::string request(buffer);
    std::cout << "<<<<< CLIENT REQUEST >>>>>" << std::endl << std::endl;
    for (int i = 0 ; i < request.size() ; i++)
        std::cout << request;
    std::cout << std::endl << std::endl;
}

