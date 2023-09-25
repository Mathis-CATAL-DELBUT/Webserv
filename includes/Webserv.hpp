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
#include <map>

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
        Webserv(Parsing* config);
        Webserv(Webserv const &copy);
        ~Webserv();
        Webserv &operator=(Webserv const &rhs);
        bool init();
        bool process();

    private:
        int _listenSd, _maxSd;
        int _serverPort;
        bool _endServ, _closeConn;
        fd_set rtmp, wtmp, rfds, wfds;
        Parsing* _config;
        std::map<int, std::pair<Request*, Response*>> clientS;

        Webserv();
        void newConnHandling();
        void existingConnHandling(int currSd);
        int receiveRequest(int currSd);
        void sendResponse(int currSd);
        void closeConn(int currSd);
        int handlingErrorInit(std::string function);
        Response*	handle_request(Parsing *config, Request *req);
};

#endif