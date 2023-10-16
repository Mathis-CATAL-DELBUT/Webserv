#ifndef Response_HPP
#define Response_HPP
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <sys/stat.h>
#include "Request.hpp"
#include "Parsing.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include "Cgi.hpp"

class Response {
	public:
		Response();
		Response(Parsing* i_config, Request* i_request);
		~Response();
		Response(const Response& cpy);
		Response&	operator=(const Response& rhs);

		std::string				getResponse();
		
	private:
		std::string			getDate();
		std::string			getFileContent(const std::string& file_path);
		int					getFileLength(std::ifstream& file);
		
		void				setMethod();
		void				setBody(const std::string& file_path);
		std::string			convertInt(int value);
		void				checkFile(const std::string& file_path);
		bool				checkDirectory(std::string& file_path);
		
		int status;
		int content_length;
		
		std::string connection;
		std::string content_type;
		std::string	file_path;
		std::string body;
		
		Parsing*	config;
		Request*	request;
};

#endif