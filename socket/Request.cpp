#include "Request.hpp"

Request::Request(char *buffer) 
{
    std::string r(buffer);
    this->rawRequest = r;
    parsingFormat();
}

Request::~Request() 
{}

Request::Request(Request const &copy) {}

Request &Request::operator=(Request const &rhs)
{
    if (this == &rhs)
        return (*this);
    return (*this);
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

std::string Request::getRessource() const
{
    return (this->_ressource);
}

std::string Request::getContentType() const
{
    return (this->_contentType);
}

std::string Request::getRawRequest() const
{
    return (this->rawRequest);
}

std::string Request::getExtension() const
{
    return (this->_extension);
}

std::string Request::infile(std::string str, char c)
{
    int i = str.size() - 1;
    for (; str[i] != '/' ; i--) {}
        return str.substr(i + 1);
}

void Request::parsingFormat()
{
    std::istringstream iss(this->rawRequest);
    std::string tmp;

    iss >> tmp;
    this->_method = (tmp == "GET" ? GET : (tmp == "DELETE" ? DELETE : POST));
    iss >> tmp;
    std::string path = infile(tmp, '/');
    this->_ressource = path;
    this->_extension = _ressource.substr(_ressource.find('.') + 1);
    iss >> this->_protocol;
    // exit(0);
}

