/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-sain <ale-sain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:23:51 by mcatal-d          #+#    #+#             */
/*   Updated: 2023/10/03 14:53:30 by ale-sain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"

Parsing::Parsing(std::string file)
{
    this->fd = open(file.c_str(), O_RDONLY);
    if (this->fd != 3)
    {
        std::cout << "Error: " << file  << ": not foud or no perm" << std::endl;
        exit(1);
    }
    if (setListen(file) || setServerName(file) || 
        setRoot(file) || setIndex(file) || 
        setErrorPage(file) || setImage(file) || 
        setHtml(file) || setWelcome(file) ||
        setCss(file) || setScript(file))
    {
        std::cout << "Error Parsing: " << file << std::endl;
        exit(1);
    }
	setExtension();
	setDefaultErrorPage();
}

Parsing::~Parsing()
{
    close(this->fd);
}

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

std::vector<std::string> Parsing::getScript() const
{return (this->script);}

const std::string&	Parsing::getDefaultErrorPage(const std::string& error_code) 
{	
	return (default_error[error_code]);
}

const std::string&	Parsing::getExtension(const std::string& extension) 
{
	return (file_extension[extension]);
}

int Parsing::setServerName(std::string file)
{
    this->server_name = parseSoloElt(file, "server_name");
    if (this->server_name == "")
        return (1);
    return (0);
}

int Parsing::setRoot(std::string file)
{
    this->root = parseSoloElt(file, "root");
    if (this->root == "")
        return (1);
    return (0);
}
int Parsing::setIndex(std::string file)
{
    this->index = parseSoloElt(file, "index");
    if (this->index == "")
        return (1);
    return (0);
}
int Parsing::setErrorPage(std::string file)
{
    this->error_page = parseMultiEltString(file, "error_page");
    if (this->error_page.size() == 0)
        return (1);
    return (0);
}

int Parsing::setHtml(std::string file)
{
    this->html = parseMultiEltString(file, "Html");
    if (this->html.size() == 0)
        return (1);
    return (0);
}

int Parsing::setWelcome(std::string file)
{
    this->welcome = parseMultiEltString(file, "welcome");
    if (this->welcome.size() == 0)
        return (1);
    return (0);
}

int Parsing::setCss(std::string file)
{
    this->css = parseMultiEltString(file, "css");
    if (this->css.size() == 0)
        return (1);
    return (0);
}

int Parsing::setScript(std::string file)
{
    this->script = parseMultiEltString(file, "script");
    if (this->script.size() == 0)
        return (1);
    return (0);
}

int Parsing::setListen(std::string file)
{
    std::string line;
    std::ifstream fd(file.c_str());
    while (getline(fd, line))
    {
        if (line.find("listen") != std::string::npos)
        {
            getline(fd, line);
            if (line.find("{") != std::string::npos)
            {
                while (getline(fd, line))
                {
                    if (line.find("}") != std::string::npos)
                        return (0);
                    else
                        this->listen.push_back(atoi(line.c_str()));
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


void Parsing::removeSpace(std::string &str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == ' ')
            str.erase(i, 1);
        else
            i++;
    }
    if (str[i - 1] != ';')
    {
        std::cout << "Parse error" << std::endl;
        exit(1);
    }
    str.erase(i - 1, 1);
}

std::string Parsing::parseSoloElt(std::string file, std::string name)
{
    std::string result;
    std::string line;
    std::ifstream fd(file.c_str());
    while (getline(fd, line))
    {
        if (line.find(name) != std::string::npos)
        {
            getline(fd, line);
            if (line.find("{") != std::string::npos)
            {
                getline(fd, line);
                result = line;
                getline(fd, line);
                if (line.find("}") != std::string::npos)
                    return (removeSpace(result), result);
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
        if (line.find(name) != std::string::npos)
        {
            getline(fd, line);
            if (line.find("{") != std::string::npos)
            {
                while (getline(fd, line))
                {
                    if (line.find("}") != std::string::npos)
                        return (result);
                    else
                    {
                        if (name == "listen")
                            result.push_back(line.c_str());
                        else
                        {
                            removeSpace(line);
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

