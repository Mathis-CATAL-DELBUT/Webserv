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

#include "Request.hpp"
#include "Response.hpp"

// class Response;
class Request;

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
        int _on;
        int _returnCode;
        int _serverPort;
        int _newSd;
        int _descReady;
        int _endServ;
        int _closeConn;
        int _run;
        fd_set _masterSet, _workingSet;
        sockaddr_in6 _addr;
        timeval _timeOut;

        Request _request;
        // Response _response;
        void newConnHandling();
        void existingConnHandling(int currSd);
        void closeConn(int currSd);
        void receiveRequest(int currSd);
        void sendResponse(int currSd);

        void handlingErrorConn();
        int handlingErrorInit(std::string function);
};

#endif