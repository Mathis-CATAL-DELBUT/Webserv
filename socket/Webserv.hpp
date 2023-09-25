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
#include <map>
#include <fcntl.h>

#include "Request.hpp"
#include "Response.hpp"

class Request;
class Response;

class Webserv
{
    public:
        Webserv();
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
        fd_set rtmp, wtmp, rfds, wfds;
        timeval _timeOut;

        Response _response;
        std::map<int, std::pair<Request*, Response*>> clientS;
        void newConnHandling();
        void existingConnHandling(int currSd);
        void closeConn(int currSd);
        int receiveRequest(int currSd);
        
        void sendResponse(int currSd);
        std::string formating(std::string content);

        int handlingErrorConn();
        int handlingErrorInit(std::string function);
};

#endif