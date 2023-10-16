#include "Request.hpp"

Request::Request() {}

Request::Request(const std::string& s_request) {
	std::string header;
	size_t header_end = s_request.find("\r\n\r\n");
	if (header_end != std::string::npos) {
		header = s_request.substr(0, header_end);
		data["body"] = s_request.substr(header_end + 4, s_request.size() - header_end - 5);
	}
	else {
		header = s_request;
		data["body"] = "";
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
	data["Method"] = line.substr(0, pos);
	if (qmark != std::string::npos) {
		data["File"] = line.substr(pos + 1, qmark - pos - 1);
		data["query"] = line.substr(qmark + 1, pos_end - qmark - 1);
	}
	else {
		data["File"] = line.substr(pos + 1, pos_end - pos - 1);
	}
}

void	Request::parseHeader(const std::string& line) {
	size_t pos = line.find(':');
	std::string key = line.substr(0, pos);
	if (pos != std::string::npos)
		data[key] = line.substr(pos + 2, line.size() - pos - 3);
}


Request::Request(const Request& cpy) {
	*this = cpy;
}

Request&	Request::operator=(const Request& rhs) {
	if (this != &rhs) {
		data = rhs.data;
	}
	return (*this);
}

void	Request::display() {
	std::map<std::string, std::string>::iterator it = data.begin();
	while (it != data.end()) {
		std::cout << it->first << " " << it->second << std::endl;
		it++;
	}
}

Request::~Request() {
	data.clear();
}