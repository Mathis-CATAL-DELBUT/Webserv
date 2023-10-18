#include "Parsing.hpp"

Parsing::Parsing(std::string file)
{
    if (open(file.c_str(), O_RDONLY) != 3)
    {
        std::cout << "Error: " << file  << ": not foud or no perm" << std::endl;
        exit(1);
    }
    if (setListen(file) || setServerName(file) || 
        setRoot(file) || setIndex(file) || 
        setErrorPage(file) || setImage(file) || 
        setHtml(file) || setWelcome(file) ||
        setCss(file) || setClientMaxBodySize(file) || 
        setDirectoryListing(file) || setMethod(file) ||
        this->listen.size() == 0)
    {
        std::cout << "Error Parsing: " << file << std::endl;
        exit(1);
    }
	setErrorName();
	setTimeout(false);
	setExtension();
	setDefaultErrorPage();
}

Parsing::~Parsing(){}

Parsing::Parsing(const Parsing &copy)
{*this = copy;}

Parsing &Parsing::operator=(const Parsing &copy)
{
    if (this != &copy)
    {
        this->listen = copy.listen;
        this->server_name = copy.server_name;
        this->root = copy.root;
        this->index = copy.index;
        this->error_page = copy.error_page;
    }
    return (*this);
}

std::vector<int> Parsing::getListen() const
{return (this->listen);}

std::string Parsing::getServerName() const
{return (this->server_name);}

std::string Parsing::getRoot() const
{return (this->root);}

std::string Parsing::getIndex() const
{return (this->index);}

std::vector<std::string> Parsing::getErrorPage()
{return (this->error_page);}

std::vector<std::string> Parsing::getImage() const
{return (this->image);}

std::vector<std::string> Parsing::getHtml() const
{return (this->html);}

std::vector<std::string> Parsing::getWelcome() const
{return (this->welcome);}

std::vector<std::string> Parsing::getCss() const
{return (this->css);}

int Parsing::getClientMaxBodySize() const
{return (this->client_max_body_size);}

std::string Parsing::getDirectoryListing() const
{return (this->directory_listing);}

const std::string&	Parsing::getDefaultErrorPage(const std::string& error_code) 
{	
	return (default_error[error_code]);
}

const std::string&	Parsing::getExtension(const std::string& extension) 
{
	return (file_extension[extension]);
}

bool    Parsing::getMethod(std::string method) const
{
    for (std::vector<std::string>::size_type i = 0; i < this->method.size(); i++)
    {
        if (this->method[i] == method)
            return (true);
    }
    return (false);
}

bool	Parsing::getTimeout() const
{
	return (_timeout);
}

void	Parsing::setTimeout(bool value) {
	_timeout = value;
}

int Parsing::setServerName(std::string file)
{
    this->server_name = parseSoloElt(file, "server_name");
    if (this->server_name == "" || checkLink(this->server_name) == 1)
        return (1);
    return (0);
}

int Parsing::setRoot(std::string file)
{
    this->root = parseSoloElt(file, "root");
    if (this->root == "" || checkLink(this->root) == 1)
        return (1);
    return (0);
}
int Parsing::setIndex(std::string file)
{
    this->index = parseSoloElt(file, "index");
    if (this->index == "" || checkLink(this->index) == 1)
        return (1);
    return (0);
}
int Parsing::setErrorPage(std::string file)
{
    this->error_page = parseMultiEltString(file, "error_page");
    if (this->error_page.size() == 0)
        return (1);
    for (std::vector<std::string>::size_type i = 0; i < this->error_page.size(); i++)
    {
        if (checkLink(this->error_page[i]) == 1)
            return (1);
    }
    return (0);
}

int Parsing::setHtml(std::string file)
{
    this->html = parseMultiEltString(file, "Html");
    if (this->html.size() == 0)
        return (1);
    for (std::vector<std::string>::size_type i = 0; i < this->html.size(); i++)
    {
        if (checkLink(this->html[i]) == 1)
            return (1);
    }
    return (0);
}

int Parsing::setWelcome(std::string file)
{
    this->welcome = parseMultiEltString(file, "welcome_page");
    if (this->welcome.size() == 0)
        return (1);
    for (std::vector<std::string>::size_type i = 0; i < this->welcome.size(); i++)
    {
        if (checkLink(this->welcome[i]) == 1)
            return (1);
    }
    return (0);
}

int Parsing::setCss(std::string file)
{
    this->css = parseMultiEltString(file, "css");
    if (this->css.size() == 0)
        return (1);
    for (std::vector<std::string>::size_type i = 0; i < this->css.size(); i++)
    {
        if (checkLink(this->css[i]) == 1)
            return (1);
    }
    return (0);
}

int Parsing::doubleListen(std::vector<int> listen)
{
    for (std::vector<int>::size_type i = 0; i < listen.size(); i++)
    {
        for (std::vector<int>::size_type j = 0; j < listen.size(); j++)
        {
            if (i != j && listen[i] == listen[j])
                return (1);
        }
    }
    return (0);
}

int Parsing::setListen(std::string file)
{
    std::string line;
    std::ifstream fd(file.c_str());
    while (getline(fd, line))
    {
        if (removeSpace(line) == "listen")
        {
            getline(fd, line);
            if (removeSpace(line) == "{")
            {
                while (getline(fd, line))
                {
                    if (removeSpace(line) == "}")
                    {
                        if (doubleListen(this->listen) == 1)
                            return (1);
                        return (0);
                    }
                    else
                    {
                        removeSpace(line, file);
                        if (onlyNumber(line) == 0)
                            this->listen.push_back(atoi(line.c_str()));
                    }
                }
                return (1);
            }
            return (1);
        }
    }
    fd.close();
    std::cout << "Error: " << file  << ": not foud or no perm" << std::endl;
    return (1);
}

int Parsing::setImage(std::string file)
{
    this->image = parseMultiEltString(file, "image");
    if (this->image.size() == 0)
        return (1);
    for (std::vector<std::string>::size_type i = 0; i < this->html.size(); i++)
    {
        if (checkLink(this->image[i]) == 1)
            return (1);
    }
    return (0);
}

int Parsing::setClientMaxBodySize(std::string file)
{
    std::string client;
    client = parseSoloElt(file, "client_max_body_size");
    if (client == "" || onlyNumber(client) == 1)
        return (1);
    this->client_max_body_size = atoi(client.c_str());
    return (0);
}

int Parsing::setDirectoryListing(std::string file)
{
    this->directory_listing = parseSoloElt(file, "directory_listing");
    if (this->directory_listing.size() == 0 || 
        (this->directory_listing != "on" && this->directory_listing != "off"))
        return (1);
    return (0);
}

int Parsing::setMethod(std::string file)
{
    this->method = parseMultiEltString(file, "method");
    if (this->method.size() == 0)
        return (1);
    for (std::vector<std::string>::size_type i = 0; i < this->method.size(); i++)
    {
        if (this->method[i] != "GET" && this->method[i] != "POST" && this->method[i] != "DELETE" && this->method[i] != "PUT" && this->method[i] != "HEAD" && this->method[i] != "CONNECT" && this->method[i] != "OPTIONS" && this->method[i] != "TRACE" && this->method[i] != "PATCH")
            return (1);
    }
    return (0);
}

void	Parsing::setDefaultErrorPage()
{
	default_error["404"] = "<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center><hr><center>Webserv</center></body></html>";
	default_error["403"] = "<html><head><title>403 Forbidden</title></head><body><center><h1>403 Forbidden</h1></center><hr><center>Webserv</center></body></html>";
	default_error["400"] = "<html><head><title>400 Bad Request</title></head><body><center><h1>400 Bad Request</h1></center><hr><center>Webserv</center></body></html>";
	default_error["405"] = "<html><head><title>405 Method Not Allowed</title></head><body><center><h1>405 Method Not Allowed</h1></center><hr><center>Webserv</center></body></html>";
	default_error["500"] = "<html><head><title>500 Internal Server Error</title></head><body><center><h1>500 Internal Server Error</h1></center><hr><center>Webserv</center></body></html>";
	default_error["501"] = "<html><head><title>501 Not Implemented</title></head><body><center><h1>501 Not Implemented</h1></center><hr><center>Webserv</center></body></html>";
	default_error["505"] = "<html><head><title>505 HTTP Version Not Supported</title></head><body><center><h1>505 HTTP Version Not Supported</h1></center><hr><center>Webserv</center></body></html>";
}

void	Parsing::setExtension() {
	file_extension["jpg"] = "image/jpeg";
	file_extension["html"] = "text/html";
    file_extension["css"] = "text/css";
    file_extension["js"] = "text/javascript";
	file_extension["webp"] = "image/webp";
	file_extension["png"] = "image/png";
	file_extension["ico"] = "image/x-icon";
	file_extension["gif"] = "image/gif";
	file_extension["svg"] = "image/svg+xml";
	file_extension["mp4"] = "video/mp4";
	file_extension["mp3"] = "audio/mpeg";
	file_extension["wav"] = "audio/wav";
	file_extension["ogg"] = "audio/ogg";
	file_extension["txt"] = "text/plain";
	file_extension["pdf"] = "application/pdf";
	file_extension["zip"] = "application/zip";
	file_extension["tar"] = "application/x-tar";
	file_extension["gz"] = "application/gzip";
	file_extension["rar"] = "application/x-rar-compressed";
	file_extension["7z"] = "application/x-7z-compressed";
	file_extension["json"] = "application/json";
	file_extension["xml"] = "application/xml";
}

void			Parsing::setErrorName()
{
	_errorname[100] = "Continue";
	_errorname[200] = "OK";
	_errorname[201] = "Created";
	_errorname[204] = "No Content";
	_errorname[301] = "Moved Permanently";
	_errorname[308] = "Permanent Redirect";
	_errorname[400] = "Bad Request";
	_errorname[403] = "Forbidden";
	_errorname[404] = "Not Found";
	_errorname[405] = "Method Not Allowed";
	_errorname[408] = "Request Timeout";
	_errorname[413] = "Payload Too Large";
	_errorname[500] = "Internal Server Error";
}

const std::string&	Parsing::getErrorName(int status) {
	return (_errorname[status]);
}


void Parsing::removeSpace(std::string &str, std::string file)
{
    int i = 0;
    while (str[i] == ' ')
        str.erase(i, 1);
    while (str[i])
            i++;
    if (str[i - 1] != ';')
    {
        std::cout << "Error Parsing: " << file << std::endl;
        exit(1);
    }
    str.erase(i - 1, 1);
}

std::string Parsing::removeSpace(std::string &str)
{
    int i = 0;
    std::string result = "";
    while (str[i])
    {
        if (str[i] != ' ')
            result += str[i];
        i++;
    }
    return (result);
}

int Parsing::onlyNumber(std::string str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (1);
        i++;
    }
    return (0);
}

int Parsing::checkLink(std::string str)
{
    int i = 0;
    while (str[i] )
    {
        if (str[i] == ' ')
            return (1);
        i++;
    }
    return (0);
}

std::string Parsing::parseSoloElt(std::string file, std::string name)
{
    std::string result;
    std::string line;
    std::ifstream fd(file.c_str());
    while (getline(fd, line))
    {
        if (removeSpace(line) == name)
        {
            getline(fd, line);
            if (removeSpace(line) == "{")
            {
                getline(fd, line);
                result = line;
                getline(fd, line);
                if (removeSpace(line) == "}")
                    return (removeSpace(result, file), result);
                return ("");
            }
            return ("");
        }
    }
    return ("");
}

std::vector<std::string> Parsing::parseMultiEltString(std::string file, std::string name)
{
    std::vector<std::string> result;
    std::string line;
    std::ifstream fd(file.c_str());
    while (getline(fd, line))
    {
        if (removeSpace(line) == name)
        {
            getline(fd, line);
            if (removeSpace(line) == "{")
            {
                while (getline(fd, line))
                {
                    if (removeSpace(line) == "}")
                        return (result);
                    else
                    {
                        if (name == "listen")
                            result.push_back(line.c_str());
                        else
                        {
                            removeSpace(line, file);
                            result.push_back(line);
                        }
                    }
                }
                return std::vector<std::string>();
            }
            return std::vector<std::string>();
        }
    }
    return std::vector<std::string>();
}

