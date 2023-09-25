/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcatal-d <mcatal-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 10:34:30 by tedelin           #+#    #+#             */
/*   Updated: 2023/09/25 18:04:40 by mcatal-d         ###   ########.fr       */
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
#include <wait.h>

class Response {
	public:
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
		// bool				checkHeaderError(const std::string& method);
		
		int status;
		int content_length;
		
		std::string content_type;
		std::string connection;
		std::string	name;
		std::string body;
		
		Parsing*	config;
		Request*	request;
		
	private:
		Response();
};

#endif