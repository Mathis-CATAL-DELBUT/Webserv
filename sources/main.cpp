/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:41:32 by mcatal-d          #+#    #+#             */
/*   Updated: 2023/09/11 14:49:45 by mcatal-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

int main()
{
    Parsing p("config/default.conf");
    std::cout << p.getServerName() << std::endl;
    return (0);
}