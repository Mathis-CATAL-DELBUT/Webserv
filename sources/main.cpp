/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:41:32 by mcatal-d          #+#    #+#             */
/*   Updated: 2023/10/11 16:01:33 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"


int main(int argc, char **argv)
{
    if (argc != 2)
        return (std::cout << "Error: number of arguments" << std::endl, 1);
    // signal(SIGINT, SIG_IGN);
    
    Parsing* config = new Parsing(argv[1]);

    Webserv ws(config);
    if (ws.initAllServ() == false)
        return 0;
    if (ws.processAllServ() == false)
        return 0;
    return (0);
}