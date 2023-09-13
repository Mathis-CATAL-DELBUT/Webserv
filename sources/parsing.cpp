/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:23:51 by mcatal-d          #+#    #+#             */
/*   Updated: 2023/09/13 11:42:13 by mcatal-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

Parsing::Parsing(std::string file)
{
    if (open(file.c_str(), O_RDONLY) != 3)
    {
        std::cout << "Error: " << file  << ": not foud or no perm" << std::endl;
        exit(1);
    }
    if (setListen(file) || setServerName(file) || setRoot(file) || setIndex(file) || setErrorPage(file))
    {
        std::cout << "Error Parsing: " << file << std::endl;
        exit(1);
    }
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

std::vector<std::string> Parsing::getErrorPage() const
{return (this->error_page);}

int Parsing::setServerName(std::string file)
{
    this->server_name = parseSoloElt(file, "server_name");
    if (this->server_name == "-1")
        return (1);
    return (0);
}

int Parsing::setRoot(std::string file)
{
    this->root = parseSoloElt(file, "root");
    if (this->root == "-1")
        return (1);
    return (0);
}
int Parsing::setIndex(std::string file)
{
    this->index = parseSoloElt(file, "index");
    if (this->index == "-1")
        return (1);
    return (0);
}
int Parsing::setErrorPage(std::string file)
{
    std::string line;
    std::ifstream fd(file.c_str());
    while (getline(fd, line))
    {
        if (line.find("error_page") != std::string::npos)
        {
            getline(fd, line);
            if (line.find("{") != std::string::npos)
            {
                while (getline(fd, line))
                {
                    if (line.find("}") != std::string::npos)
                        return (0);
                    else
                        this->error_page.push_back(line);
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
    if (str[i - 1] == ';')
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
                return ("-1");
            }
            return ("-1");
        }
    }
    return ("-1");
}

