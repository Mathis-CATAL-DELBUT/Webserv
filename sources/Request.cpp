/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 14:20:02 by tedelin           #+#    #+#             */
/*   Updated: 2023/09/24 17:30:33 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() {}

Request::Request(const std::string& s_request) {
	size_t i = 0;
	while (s_request[i] != '\0') {
		if (i == 0 ) {
			size_t pos = s_request.find(' ', i);
			size_t pos_end = s_request.find(' ', pos + 1);
			_data["Method"] = s_request.substr(i, pos);
			_data["File"] = s_request.substr(pos + 1, pos_end - pos - 1);
			i = s_request.find('\n', i) + 1;
		}
		else if (s_request.find(':', i) != std::string::npos) { 
			size_t pos = s_request.find(':', i);
			size_t line_end = s_request.find('\n', i);
			if (line_end == std::string::npos)
				line_end = s_request.size();
			std::string key = s_request.substr(i, pos - i);
			_data[key] = s_request.substr(pos + 2, line_end - pos - 2);
			i = line_end + 1;
		}
		else
			break;
	}
}

Request::Request(const Request& cpy) {
	*this = cpy;
}

Request&	Request::operator=(const Request& rhs) {
	if (this != &rhs) {
		_data = rhs._data;
	}
	return (*this);
}

std::string	Request::getValue(const std::string& key) {
	std::map<std::string, std::string>::iterator it = _data.find(key);
	if (it != _data.end()) {
		return (_data[key]);
	}
	return ("");
}

void	Request::display() {
	std::map<std::string, std::string>::iterator it = _data.begin();
	while (it != _data.end()) {
		std::cout << it->first << " " << it->second << std::endl;
		it++;
	}
}

Request::~Request() {
	_data.clear();
}