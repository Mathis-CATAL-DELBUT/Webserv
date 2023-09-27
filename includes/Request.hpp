/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 17:44:48 by tedelin           #+#    #+#             */
/*   Updated: 2023/09/24 17:30:33 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <map>
#include <iostream>

// GET /image/norminet.jpg HTTP/1.1
// Host: localhost:85
// User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0
// Accept: image/avif,image/webp,*/*
// Accept-Language: en-US,en;q=0.5
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive
// Referer: http://localhost:85/norminet.html
// Sec-Fetch-Dest: image
// Sec-Fetch-Mode: no-cors
// Sec-Fetch-Site: same-origin

class Request {
	public:
		Request();
		Request(const std::string& s_request);
		~Request();
		Request&	operator=(const Request& rhs);
		Request(const Request& cpy);
		
		std::string	getValue(const std::string& key);
		
	private:
		void	display();
		void	parseRequest();
		std::map<std::string, std::string> _data;
		
};

#endif