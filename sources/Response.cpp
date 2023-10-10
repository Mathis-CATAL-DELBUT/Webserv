/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 15:33:33 by tedelin           #+#    #+#             */
/*   Updated: 2023/10/09 15:36:40 by mcatal-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Request.hpp"

Response::Response() {}

Response::Response(Parsing* i_config, Request* i_request) : config(i_config), request(i_request) {
	status = 200;
	connection = request->getValue("Connection");
	body = "";
	if (request->getValue("File").find("CGI") != std::string::npos) {
		if (request->getValue("boundary") != "") {
			upload_file();
		}
		else {
			doCGI();
		}
	}
	else
	{
		content_type = config->getExtension(&(request->getValue("File"))[request->getValue("File").find(".") + 1]);
		content_length = 0;
	}
	if (content_type == "")
		status = 415;
}

Response::Response(const Response& cpy) {
	*this = cpy;
}

Response&	Response::operator=(const Response& rhs) {
	if (this != &rhs) {
	}
	return (*this);
}

void	Response::upload_file() {
	std::string file_path = config->getRoot() + request->getValue("File") + "/" + request->getValue("file_name");
	std::cout << "PATH:" << file_path << std::endl;
	std::ofstream file(file_path.c_str());
	file << request->getValue("Body");
	file.close();
}

pid_t Response::write_stdin(int *fd_in, int *fd_out) {
	char * info = new char[request->getValue("form").size() + 1];
	strcpy(info, request->getValue("form").c_str());
	char *const args[] = { (char*)"/usr/bin/echo", info, NULL };
    char *const env[] = { NULL };
    pid_t child_pid = fork();
    if (child_pid == 0) {
        dup2(*fd_out, 1);
		close(*fd_in);
		close(*fd_out);
        execve("/usr/bin/echo", args, env);
    } else {
		close(*fd_out);
    }
	return child_pid;
}

pid_t	Response::exec_script(int *fd_in, int *fd_out) {
	char **env = NULL;
	char **av = NULL;
	pid_t child_pid = fork();
	if (child_pid == 0) {
		dup2(*fd_in, STDIN_FILENO);
		close(*fd_in);
		if (execve(("data" + std::string(request->getValue("File"))).c_str(), av, env) == -1)
			exit(EXIT_FAILURE);
	} else {
       	close(*fd_in);
        close(*fd_out);
	}
	return (child_pid);
}

void	Response::doCGI()
{
	unlink("data/CGI/.CGI.txt");
	int fd[2];
	pipe(fd);
	int state;
	waitpid(write_stdin(&fd[0], &fd[1]), &state, 0);
	waitpid(exec_script(&fd[0], &fd[1]), NULL, 0);
	std::fstream file("data/CGI/.CGI.txt");
	std::string filePath = "data/CGI/.CGI.txt";
	body = getFileContent(filePath);
	content_type = "text/html";
	content_length = body.size();
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