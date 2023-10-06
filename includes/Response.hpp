/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 10:34:30 by tedelin           #+#    #+#             */
/*   Updated: 2023/10/06 11:57:41 by mcatal-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

class Response {
	public:
		Response();
		Response(Parsing* i_config, Request* i_request);
		~Response();
		Response(const Response& cpy);
		Response&	operator=(const Response& rhs);

		std::string				getResponse();
		
	protected:
		void				checkFile(const std::string& file_path);
		std::string			getDate();
		std::string			getFileContent(const std::string& file_path);
		int					getFileLength(std::ifstream& file);
		void				setBody(const std::string& file_path);
		void				sendHeaderError();
		std::string			convertInt(int value);
		void				doCGI();
		pid_t				write_stdin(int *fd_in, int *fd_out);
		pid_t				exec_script(int *fd_in, int *fd_out);
		// bool				checkHeaderError(const std::string& method);
		
		int status;
		int content_length;
		
		std::string content_type;
		std::string connection;
		std::string	name;
		std::string body;
		
		Parsing*	config;
		Request*	request;
};

#endif