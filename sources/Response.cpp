#include "Response.hpp"
#include "Request.hpp"
#include <stdio.h>
#include <dirent.h>
#include <sstream>

Response::Response() {}

Response::Response(Parsing* i_config, Request* i_request) : config(i_config), request(i_request) {
	status = 200;
	file_path = config->getRoot() + request->data["File"];
	body = "";
	setMethod();
}

void	Response::setMethod() { 
	// Need to add config file protection 
	// if (std::atoi(request->data["Content-Length"].c_str()) > config->getClientMaxBodySize())
	// 	status = 413;
	if ((config->getMethod("GET") || config->getMethod("POST") || config->getMethod("DELETE")) && status == 200) {
		if (request->data["Method"] == "GET" || request->data["Method"] == "POST") {
			if (request->data["File"].find("CGI") != std::string::npos) {
				Cgi(request, config);
				std::string filePath = "data/CGI/.CGI.txt";
				body = getFileContent(filePath);
				content_type = "text/html";
				content_length = body.size();
			} else {
				content_type = config->getExtension(&(request->data["File"])[request->data["File"].find(".") + 1]);
				content_length = 0;
			}		
		}
		if (request->data["Method"] == "GET") {
			std::string query = request->data["File"];
			if (checkDirectory(query) == false) {
				checkFile(file_path);
				setBody(file_path);
			}
		}
		else if (request->data["Method"] == "DELETE") {
			checkFile(file_path);
			if (status == 200) {
				remove(file_path.c_str());
			}
		}
	}
	if (content_type == "")
		status = 415;
}

bool Response::checkDirectory(std::string& file_path) {
	std::stringstream response;
    if (config->getDirectoryListing() == "on" && file_path != "/") {
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
				if (request->data["Method"] == "DELETE")
					status = 403;
			}
			else {
				content_length = getFileLength(file);
				content_type = config->getExtension(&(request->data["File"])[request->data["File"].find(".") + 1]);
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