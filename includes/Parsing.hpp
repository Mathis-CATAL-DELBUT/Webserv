/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:23:59 by mcatal-d          #+#    #+#             */
/*   Updated: 2023/10/11 15:44:22 by mcatal-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <map>

class Parsing
{
    public:
        Parsing(std::string file);
        ~Parsing();
        Parsing &operator=(const Parsing &copy);
        Parsing(const Parsing &copy);
        
        int                         getClientMaxBodySize() const;
        bool                        getMethod(std::string methode) const;
        std::vector<int>            getListen() const;
        std::string                 getServerName() const;
        std::string                 getDirectoryListing() const;
        std::string                 getRoot() const;
        std::string                 getIndex() const;
        std::vector<std::string>    getErrorPage();
        std::vector<std::string>    getImage() const;
        std::vector<std::string>    getHtml() const;
        std::vector<std::string>    getWelcome() const;
        std::vector<std::string>    getCss() const;
		const std::string&		    getDefaultErrorPage(const std::string& error_code);
		const std::string&		    getExtension(const std::string& extension);
        
    private:
        int                                 client_max_body_size;
        std::vector<int>                    listen;
        std::vector<std::string>            error_page;  
        std::vector<std::string>            method;  
        std::vector<std::string>            image;  
        std::vector<std::string>            html;  
        std::vector<std::string>            welcome;  
        std::vector<std::string>            css;  
        std::string                         server_name;
        std::string                         directory_listing;
        std::string                         root;
        std::string                         index;
		std::map<std::string, std::string>	default_error;
		std::map<std::string, std::string>	file_extension;

        int     setMethod(std::string file);
        int     setDirectoryListing(std::string file);
        int     setClientMaxBodySize(std::string file);
        int     setListen(std::string file);
        int     setServerName(std::string file);
        int     setRoot(std::string file);
        int     setIndex(std::string file);
        int     setErrorPage(std::string file);
        int     setImage(std::string file);
        int     setHtml(std::string file);
        int     setWelcome(std::string file);
        int     setCss(std::string file);
		void	setDefaultErrorPage();
		void	setExtension();

        void removeSpace(std::string &str, std::string file);
        std::string removeSpace(std::string &str);
        std::string  parseSoloElt(std::string file, std::string name);
        std::vector<std::string> parseMultiEltString(std::string file, std::string name);
        int onlyNumber(std::string str);
        int checkLink(std::string str);
};

#endif