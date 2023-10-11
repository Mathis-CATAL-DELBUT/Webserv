/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tedelin <tedelin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:33 by tedelin           #+#    #+#             */
/*   Updated: 2023/10/11 16:19:25 by tedelin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Request.hpp"
#include <stdio.h>
#include <dirent.h>
#include <sstream>

Response::Response() {}

Response::Response(Parsing* i_config, Request* i_request) : config(i_config), request(i_request) {
	status = 200;
	connection = request->getValue("Connection");
	body = "";
	setMethod();
	if (content_type == "") {
			status = 415;
	}
}

void	Response::setMethod() {
	std::vector<std::string> _implemented_method;
	std::string r_method = request->getValue("Method");
	std::string file_path = config->getRoot() + request->getValue("File");
	// Need to add config file protection 
	if (r_method == "GET" || r_method == "POST") {
		if (request->getValue("File").find("CGI") != std::string::npos) {
			Cgi(request, config);
			std::string filePath = "data/CGI/.CGI.txt";
			body = getFileContent(filePath);
			content_type = "text/html";
			content_length = body.size();
		} else {
			content_type = config->getExtension(&(request->getValue("File"))[request->getValue("File").find(".") + 1]);
			content_length = 0;
		}		
	}
	if (r_method == "GET") {
		if (checkDirectory(request->getValue("File")) == false) {
			checkFile(file_path);
			setBody(file_path);
		}
	}
	else if (r_method == "DELETE") {
		checkFile(file_path);
		if (status == 200) {
			remove(file_path.c_str());
		}
	}
}

bool Response::checkDirectory(std::string& file_path) {
	std::stringstream response;
    if (config->getDirectoryListing() == "on") {
        struct dirent *de;
        DIR* dr = opendir((config->getRoot() + file_path).c_str());
        if (dr == NULL)
            return false;
		std::string path = file_path;
		if (path[path.size() - 1] != '/')
			path += "/";
		if (path.size() - 1 == '/')
			path.erase(0, 1);
        response << "<html><head><link rel='stylesheet' href='../style.css'><title>Directory Listing</title></head><body><h1>You have entered a directory, here are the files it contains:</h1><br/>";
		while ((de = readdir(dr)) != NULL)
		{
			if (de->d_name[0] != '.')
            	response << "<a class=directory href='" << path << de->d_name << "'>" << de->d_name << "</a><br/>";
			else 
				response << "<p class=directory_ano >" << de->d_name << "<p/><br/>" << std::endl;
		}
        response << "</body></html>";
		body = response.str();
		content_length = body.size();
        closedir(dr);
        return true;
    } 
	else 
	{
        struct dirent *de;
        DIR* dr = opendir((config->getRoot() + file_path).c_str());
        if (dr != NULL) 
			file_path = "/welcome_page/welcome_page.html";
    }
	return false;
}


Response::Response(const Response& cpy) {
	*this = cpy;
}

Response&	Response::operator=(const Response& rhs) {
	if (this != &rhs) {
	}
	return (*this);
}

std::string Response::getDate() {
    std::time_t now = std::time(NULL);
    struct std::tm* timeinfo = std::gmtime(&now);

    const char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    const char* daysOfWeek[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

    std::ostringstream ss;
    ss << daysOfWeek[timeinfo->tm_wday] << ", "
       << std::setw(2) << std::setfill('0') << timeinfo->tm_mday << " "
       << months[timeinfo->tm_mon] << " "
       << (timeinfo->tm_year + 1900) << " "
       << std::setw(2) << std::setfill('0') << timeinfo->tm_hour << ":"
       << std::setw(2) << std::setfill('0') << timeinfo->tm_min << ":"
       << std::setw(2) << std::setfill('0') << timeinfo->tm_sec << " GMT";

    return ss.str();
}

void	Response::checkFile(const std::string& file_path) {
	std::ifstream file(file_path.c_str(), std::ios::binary | std::ios::ate);
	if (file.is_open()) {
		struct stat fileInfo;
		if (stat(file_path.c_str(), &fileInfo) == 0) {
			if (!(fileInfo.st_mode & S_IRUSR)) {
				status = 403;
			}
			if (!(fileInfo.st_mode & S_IWUSR)) {
				if (name == "DELETE")
					status = 403;
			}
			else {
				content_length = getFileLength(file);
				if (content_length == 0)
					status = 204;
				file.close();
			}
		}
	}
	else {
		status = 404;
	}
}

int	Response::getFileLength(std::ifstream& file) {
	if (body != "")
		return (body.size());
	return (file.tellg());
}

std::string Response::getFileContent(const std::string& file_path) {
	std::ifstream file(file_path.c_str());
	std::stringstream iss;
	iss << file.rdbuf();
	file.close();
	return (iss.str());
}

std::string	Response::convertInt(int value) {
	std::ostringstream oss;
    oss << value;
    return (oss.str());
}

void	Response::setBody(const std::string& file_path) {
	if (body == "")
		body = getFileContent(file_path);
	if (body.size() == 0 && status == 200) {
		status = 204;
	}
	else if (body == "" && status != 200) {
		std::string file_path = config->getRoot() + "/error_page/" + convertInt(status) + ".html";
		std::ifstream file(file_path.c_str());
		if (file.is_open()) 
			body = getFileContent(file_path);
		else
			body = config->getDefaultErrorPage(convertInt(status));
		content_length = body.size();
		content_type = "text/html";
	}
}

std::string Response::getResponse() {
	std::stringstream buff;

	buff << "HTTP/1.1 " << status << std::endl;
	buff << "Server: " << "Webserv" << std::endl;
	buff << "Date: " << getDate() << std::endl;
	buff << "Content-Type: " << content_type << std::endl;
	buff << "Content-Length: " << content_length << std::endl;
	buff << "Connection: " << connection << std::endl;
	buff << std::endl << body;
	
	return (buff.str());
}

Response::~Response() {}