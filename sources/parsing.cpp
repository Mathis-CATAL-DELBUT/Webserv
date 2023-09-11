/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:23:51 by mcatal-d          #+#    #+#             */
/*   Updated: 2023/09/11 15:42:44 by mcatal-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

Parsing::Parsing(std::string file)
{
    if (open(file.c_str(), O_RDONLY) != 3)
    {
        std::cout << "Error: " << file << std::endl;
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

