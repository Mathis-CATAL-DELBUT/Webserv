#include "Cgi.hpp"

Cgi::Cgi(Request *request, Parsing *config) : _request(request), _config(config) {
	setEnv();
	doCGI();
}

void Cgi::setEnv()
{
    _env["AUTH_TYPE"] = _request->data["Authorization"];
    _env["REDIRECT_STATUS"] = "200";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["SCRIPT_NAME"] = _config->getRoot() + _request->data["File"];
    _env["SCRIPT_FILENAME"] = _config->getRoot() + _request->data["File"];
    _env["REQUEST_METHOD"] = _request->data["Method"];
	_env["CONTENT_LENGTH"] = _request->data["Content-Length"];
    _env["CONTENT_TYPE"] = _request->data["Content-Type"];
	_env["QUERY_STRING"] = _request->data["query"];
    _env["REMOTE_ADDR"] = _config->getServerName();
    _env["REMOTE_IDENT"] = _request->data["Authorization"];
    _env["REMOTE_USER"] = _request->data["Authorization"];
    _env["SERVER_NAME"] = _config->getServerName();
    _env["SERVER_PORT"] = "8043";
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["SERVER_SOFTWARE"] = "Webserv";
	_cgi_env = mapToTab();
}

char **Cgi::mapToTab()
{
	char **CgiEnv = new char *[_env.size() + 1];
	std::string tempStr;
	int i = 0;
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++, i++)
	{
		tempStr = it->first + "=" + it->second;
		CgiEnv[i] = new char[tempStr.size() + 1];
		CgiEnv[i] = std::strcpy(CgiEnv[i], tempStr.c_str());
	}
	CgiEnv[i] = NULL;
	return CgiEnv;
}

pid_t Cgi::writeStdin(int *fd_in, int *fd_out) {
	char * info = new char[_request->data["body"].size() + 1];
	std::strcpy(info, _request->data["body"].c_str());
	char *const args[] = { (char*)"/usr/bin/echo", info, NULL };
    char *const env[] = { NULL };
    pid_t child_pid = fork();
    if (child_pid == 0) {
        dup2(*fd_out, 1);
		close(*fd_in);
		close(*fd_out);
        if (execve("/usr/bin/echo", args, env) == -1)
			exit(500);
    } else {
		close(*fd_out);
    }
	return child_pid;
}

pid_t	Cgi::execScript(int *fd_in, int *fd_out) {
	char **av = NULL;
	pid_t child_pid = fork();
	if (child_pid == 0) {
		dup2(*fd_in, STDIN_FILENO);
		close(*fd_in);
		if (execve((_config->getRoot() + std::string(_request->data["File"])).c_str(), av, _cgi_env) == -1)
			exit(500);
	} else {
       	close(*fd_in);
        close(*fd_out);
	}
	return (child_pid);
}

void	Cgi::doCGI()
{
	int fd[2];
	if (pipe(fd) == -1) {
	} 
	else {
		int w_status, e_status;
		waitpid(writeStdin(&fd[0], &fd[1]), &w_status, 0);
		waitpid(execScript(&fd[0], &fd[1]), &e_status, 0);
		if (w_status == 500 || e_status == 500) {
		}
	}
}

Cgi::~Cgi() {
	delete [] _cgi_env;
}