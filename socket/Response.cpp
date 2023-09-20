#include "Response.hpp"

Response::Response() {}

Response::~Response() {}

Response::Response(Response const &copy) {}

Response &Response::operator=(Response const &rhs)
{
    if (this == &rhs)
        return (*this);
    return (*this);
}

void Response::generateResponse(Request &request)
{
    getPathToContent(request);
    getContent();
    addingFormat(request);

    //print data from request and response built
    // std::cout << "EXTENSION = " << request.getExtension() << std::endl;
    // std::cout << "ressource = " << request.getRessource() << std::endl;
    // std::cout << "PATH = " << this->_pathToContent << std::endl;
    // std::cout << this->_response << std::endl;
}

void Response::configHeader(Request &request)
{
    this->_statusCode = 200;
    this->_statusText = "OK";
    this->_protocol = request.getProtocol();
}

void Response::configBody(Request &request)
{
    this->_modifExtension = (request.getExtension() == "js" ? "javascript" : (request.getExtension() == "jpg" ? "jpeg" : request.getExtension()));
    std::unordered_map<std::string, std::string> map{
        {"webp", "image"},
        {"jpeg", "image"},
        {"html", "text"},
        {"javascript", "application"},
        {"css", "text"}
    };
    this->_contentType = map[this->_modifExtension];
    this->_contentLength = this->_rawContent.length();
}

void Response::buildingFormattedResponse()
{
    std::ostringstream response;
    response << this->_protocol << " " << this->_statusCode << " " << this->_statusText << "\r\n";
    response << "Content-Type: " << this->_contentType << "/" << this->_modifExtension << "\r\n";
    response << "Content-Length: " << this->_contentLength << "\r\n";
    response << "\r\n" << this->_rawContent;
    this->_response = response.str();
}

void Response::addingFormat(Request &request)
{
    configHeader(request);
    configBody(request);
    buildingFormattedResponse();
}

void Response::getPathToContent(Request &request)
{
    this->_pathToContent = "../html/";
    if (request.getExtension() == "webp" || request.getExtension() == "jpg")
        this->_pathToContent += "image/";
    else
        this->_pathToContent += "data/";
    this->_pathToContent += request.getRessource();
}

void Response::getContent()
{
    std::ifstream ifs(this->_pathToContent.c_str());
    std::stringstream iss;
    iss << ifs.rdbuf();
    this->_rawContent = iss.str();
}

std::string Response::getResponse() const
{
    return (this->_response);
}

int Response::getContentLength() const
{
    return (this->_contentLength);
}