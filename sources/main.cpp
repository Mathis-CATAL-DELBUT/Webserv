/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:41:32 by mcatal-d          #+#    #+#             */
/*   Updated: 2023/09/21 11:59:36 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
        return (std::cout << "Error: number of arguments" << std::endl, 1);
    Parsing p(argv[1]);

    // Affichage des données parsées
    for (int i = 0; p.getListen()[i] ; i++)
        std::cout << "listen: '" << p.getListen()[i] << "'" << std::endl;
    std::cout << std::endl;
    std::cout << "server_name: '" << p.getServerName() << "'" << std::endl;
    std::cout << std::endl;
    std::cout << "root: '" << p.getRoot() << "'" << std::endl;
    std::cout << std::endl;
    std::cout << "index: '" << p.getIndex() << "'" << std::endl;
    int j = p.getErrorPage().size();
    std::cout << std::endl;
    for (int i = 0; i != j ; i++)
        std::cout << "error_page: '" << p.getErrorPage()[i] << "'" << std::endl;
    j = p.getImage().size();
    std::cout << std::endl;
    for (int i = 0; i != j ; i++)
        std::cout << "image: '" << p.getImage()[i] << "'" << std::endl;
    j = p.getHtml().size();
    std::cout << std::endl;
    for (int i = 0; i != j ; i++)
        std::cout << "html: '" << p.getHtml()[i] << "'" << std::endl;
    j = p.getWelcome().size();
    std::cout << std::endl;
    for (int i = 0; i != j ; i++)
        std::cout << "welcome: '" << p.getWelcome()[i] << "'" << std::endl;
    j = p.getCss().size();
    std::cout << std::endl;
    for (int i = 0; i != j ; i++)
        std::cout << "css: '" << p.getCss()[i] << "'" << std::endl;
    j = p.getScript().size();
    std::cout << std::endl;
    for (int i = 0; i != j ; i++)
        std::cout << "script: '" << p.getScript()[i] << "'" << std::endl;
    return (0);
}