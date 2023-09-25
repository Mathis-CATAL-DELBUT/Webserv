#ifndef WEBSERV_H
# define WEBSERV_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <fcntl.h>

#include "Request.hpp"
#include "Response.hpp"
#include "Parsing.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

class Request;
class Response;

class Webserv
{
    public:
        Webserv();
        Webserv(Parsing* config);
        Webserv(Webserv const &copy);
        ~Webserv();
        Webserv &operator=(Webserv const &rhs);
        bool init();
        bool process();

    private:
        int _listenSd;
        int _maxSd;
        int _returnCode;
        int _serverPort;
        int _newSd;
        int _descReady;
        int _endServ;
        int _closeConn;
        int _run;
        fd_set fds, rfds;
        timeval _timeOut;

        Request* _request;
        Response* _response;
        Parsing* _config;
        void newConnHandling();
        void existingConnHandling(int currSd);
        void closeConn(int currSd);
        int receiveRequest(int currSd);
        Response*	handle_request(Parsing *config, Request *req);
        
        int sendResponse(int currSd);
        std::string formating(std::string content);

        int handlingErrorConn();
        int handlingErrorInit(std::string function);
};

#endif