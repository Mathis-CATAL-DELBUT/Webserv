/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 14:20:02 by tedelin           #+#    #+#             */
/*   Updated: 2023/10/04 11:32:13 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() {}

Request::Request(const std::string& s_request) {
	std::string line;
	std::istringstream iss(s_request);
	size_t i = 0;
	while (getline(iss, line)) {
		if (i == 0 ) {
			size_t pos = line.find(' ');
			size_t pos_end = line.find(' ', pos + 1);
			_data["Method"] = line.substr(0, pos);
			_data["File"] = line.substr(pos + 1, pos_end - pos - 1);
		}
		if (line.find("boundary=") != std::string::npos) {
			size_t pos = line.find('=');
			_data["boundary"] = line.substr(pos + 1, line.size() - pos - 2);
		}
		else if (line.find(':') != std::string::npos) { 
			size_t pos = line.find(':');
			std::string key = line.substr(0, pos);
			_data[key] = line.substr(pos + 2, line.size() - pos - 3);
		}
		else if (line.size() == 1) {
			getline(iss, line);
			if (_data["boundary"] != "" && line.find(_data["boundary"]) != std::string::npos) {
				getline(iss, line);
				while (getline(iss, line)) {
					if (line.size() == 1) {
						while (getline(iss, line)) {
							if (line == "--" + _data["boundary"] + "--\r") {
								break;
							}
							_data["Body"] += line;
						}
						break;
					} else {
						size_t pos = line.find(':');
						std::string key = line.substr(0, pos);
						_data["file_" + key] = line.substr(pos + 2, line.size() - pos - 3);
					}
				}
			}
			else {
				_data["form"] = line;
			}
		}
		i++;
	}
	display();
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