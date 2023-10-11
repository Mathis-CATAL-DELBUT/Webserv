#include "Cgi.hpp"

Cgi::Cgi(Request *request, Parsing *config) : _request(request), _config(config) {
	doCGI();
}

pid_t Cgi::writeStdin(int *fd_in, int *fd_out) {
	char * info = new char[_request->getValue("form").size() + 1];
	strcpy(info, _request->getValue("form").c_str());
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

void	Cgi::uploadFile() {
	std::string file_path = _config->getRoot() + _request->getValue("File") + "/" + _request->getValue("file_name");
	std::cout << "PATH:" << file_path << std::endl;
	std::ofstream file(file_path.c_str());
	file << _request->getValue("Body");
	file.close();
}


pid_t	Cgi::execScript(int *fd_in, int *fd_out) {
	char **env = NULL;
	char **av = NULL;
	pid_t child_pid = fork();
	if (child_pid == 0) {
		dup2(*fd_in, STDIN_FILENO);
		close(*fd_in);
		if (execve(("data" + std::string(_request->getValue("File"))).c_str(), av, env) == -1)
			exit(EXIT_FAILURE);
	} else {
       	close(*fd_in);
        close(*fd_out);
	}
	return (child_pid);
}

void	Cgi::doCGI()
{
	if (_request->getValue("boundary") != "") {
		uploadFile();
	}
	else {
		unlink("data/CGI/.CGI.txt");
		int fd[2];
		pipe(fd);
		int state;
		waitpid(writeStdin(&fd[0], &fd[1]), &state, 0);
		waitpid(execScript(&fd[0], &fd[1]), NULL, 0);
		std::fstream file("data/CGI/.CGI.txt");
		std::string filePath = "data/CGI/.CGI.txt";
	}	
}

Cgi::~Cgi() {}