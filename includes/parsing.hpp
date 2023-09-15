/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:23:59 by mcatal-d          #+#    #+#             */
/*   Updated: 2023/09/15 10:29:48 by mcatal-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

class Parsing
{
    public:
        Parsing(std::string file);
        ~Parsing();
        Parsing &operator=(const Parsing &copy);
        Parsing(const Parsing &copy);

        std::vector<int> getListen() const;
        std::string getServerName() const;
        std::string getRoot() const;
        std::string getIndex() const;
        std::vector<std::string> getErrorPage() const;
        std::vector<std::string> getImage() const;
        std::vector<std::string> getHtml() const;
        std::vector<std::string> getWelcome() const;
        std::vector<std::string> getCss() const;
        std::vector<std::string> getScript() const;
        
    private:
        std::vector<int> listen;
        std::vector<std::string> error_page;  
        std::vector<std::string> image;  
        std::vector<std::string> html;  
        std::vector<std::string> welcome;  
        std::vector<std::string> css;  
        std::vector<std::string> script;  
        std::string server_name;
        std::string root;
        std::string index;

        int setListen(std::string file);
        int setServerName(std::string file);
        int setRoot(std::string file);
        int setIndex(std::string file);
        int setErrorPage(std::string file);
        int setImage(std::string file);
        int setHtml(std::string file);
        int setWelcome(std::string file);
        int setCss(std::string file);
        int setScript(std::string file);

        void removeSpace(std::string &str);
        std::string  parseSoloElt(std::string file, std::string name);
        std::vector<std::string> parseMultiEltString(std::string file, std::string name);
};