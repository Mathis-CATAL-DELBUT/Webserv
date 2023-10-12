#include "Request.hpp"

Request::Request() {}

Request::Request(const std::string& s_request) {
	size_t header_end = s_request.find("\r\n\r\n");
	std::string header = s_request;
	if (header_end != std::string::npos) {
		std::string header = s_request.substr(0, header_end);
		_data["body"] = s_request.substr(header_end + 4, s_request.size() - header_end - 4);
	}
	std::istringstream iss(header);
	std::string line;
	getline(iss, line);
	parseFirstLine(line);
	while (getline(iss, line)) {
		if (line.find(':') != std::string::npos) { 
			parseHeader(line);
		}
	}
	// display();
}

void	Request::parseFirstLine(const std::string& line) {
	size_t pos = line.find(' ');
	size_t pos_end = line.find(' ', pos + 1);
	size_t qmark =  line.find('?');
	_data["Method"] = line.substr(0, pos);
	if (qmark != std::string::npos)
		_data["File"] = line.substr(pos + 1, qmark - pos - 1);
	else
		_data["File"] = line.substr(pos + 1, pos_end - pos - 1);
	if (qmark != std::string::npos)
		_data["body"] = line.substr(qmark + 1, pos_end - qmark - 1);
}

void	Request::parseHeader(const std::string& line) {
	size_t pos = line.find(':');
	std::string key = line.substr(0, pos);
	_data[key] = line.substr(pos + 2, line.size() - pos - 3);
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