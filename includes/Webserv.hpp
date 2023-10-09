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
#include <ctime>

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

        bool initAllServ();
        bool processAllServ();


    private:
        int _maxSd;
        bool _endServ, _closeConn, _timeout;
        fd_set rtmp, wtmp, rfds, wfds;
        Parsing* _config;
        std::map<int, std::pair<Request*, Response*>> clientS;
        std::vector<int> ports;
        std::map<int, int> serverS;
        std::stringstream iss;

        Webserv();

        int recving(int currSd, std::string *req);
        int         initServ(int port);
        void        newConnHandling(int serverSd);
        void        existingConnHandling(int currSd);
        int         receiveRequest(int currSd);
        void        sendResponse(int currSd);
        void        closeConn(int currSd);
        int         stillNeedRecv(char *bf);
        int         handlingErrorInit(std::string function, int listenSd);
        Response*	handle_request(Parsing *config, Request *req);
};

#endif