#ifndef CGI_HPP
#define CGI_HPP
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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
		void	doCGI();

	private:
		Cgi();
		Request*	_request;
		Parsing*	_config;

};

#endif