#ifndef CGI_HPP
#define CGI_HPP
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>

#include "Request.hpp"
#include "Parsing.hpp"

class Cgi {
	public:
		Cgi(Request* request, Parsing *config);
		~Cgi();
		Cgi(const Cgi& cpy);
		Cgi&	operator=(const Cgi& rhs);

		pid_t	writeStdin(int *fd_in, int* fd_out);
		pid_t	execScript(int *fd_int, int* fd_out);
		void	uploadFile();
		void	doCGI();

	private:
		Cgi();
		Request*	_request;
		Parsing*	_config;

};

#endif