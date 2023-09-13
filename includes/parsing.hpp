/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:23:59 by mcatal-d          #+#    #+#             */
/*   Updated: 2023/09/13 11:33:17 by mcatal-d         ###   ########.fr       */
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
        
    private:
        std::vector<int> listen;
        std::string server_name;
        std::string root;
        std::string index;
        std::vector<std::string> error_page;  

        int setListen(std::string file);
        int setServerName(std::string file);
        int setRoot(std::string file);
        int setIndex(std::string file);
        int setErrorPage(std::string file);

        void removeSpace(std::string &str);
        std::string  parseSoloElt(std::string file, std::string name);
        // int  parseMultiElt(std::string &str, std::vector<std::string> &elt);
};