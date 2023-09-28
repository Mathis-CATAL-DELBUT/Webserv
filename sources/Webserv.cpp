#include "Webserv.hpp"

Webserv::Webserv(Parsing* config) : _endServ(false), ports({8080, 8081}), _timeout(false)
{
    _config = config;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&rtmp);
    FD_ZERO(&wtmp);
}

Webserv::~Webserv() {}

Webserv::Webserv(Webserv const &copy) {}

Webserv &Webserv::operator=(Webserv const &rhs)
{
    if (this == &rhs)
        return (*this);
    return (*this);
}

int Webserv::handlingErrorInit(std::string function, int listenSd)
{
    std::cerr << function << "() failed" << std::endl;
    if (listenSd > 0)
        close(listenSd);
    return (0);
}

bool Webserv::initAllServ()
{
    int fd, i = 0;
    for (; i < this->ports.size() ; i++)
    {
        fd = initServ(ports[i]);
        this->serverS[fd] = this->ports[i];
        FD_SET(fd, &(this->rfds));
    }
    this->_maxSd = fd;
    return 1;
}

int Webserv::initServ(int port)
{
    sockaddr_in6 addr;
    int _listenSd, rc, on = 1;

   _listenSd = socket(AF_INET6, SOCK_STREAM, 0);
    if (_listenSd < 0)
        return (handlingErrorInit("listen", _listenSd));
    rc = setsockopt(_listenSd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    if (rc < 0)
        return (handlingErrorInit("setsockopt", _listenSd));
    rc = fcntl(_listenSd, F_SETFL, O_NONBLOCK);
    if (rc < 0)
        return (handlingErrorInit("fcntl", _listenSd));
    std::memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port = htons(port);
    rc = bind(_listenSd, (struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0)
        return (handlingErrorInit("bind", _listenSd));

    rc = listen(_listenSd, 32);
    if (rc < 0)
        return (handlingErrorInit("listen", _listenSd));
    return (_listenSd);
}
bool Webserv::processAllServ()
{
    int rc;
    timeval timeout;

    timeout.tv_sec = 20;
    timeout.tv_usec = 0;

    while (!_endServ)
    {
        timeout.tv_sec = 3 * 60;
        timeout.tv_usec = 0;
        rtmp = rfds;
        wtmp = wfds;
        for (int i = 3 ; i <= _maxSd ; i++)
        {
            if (FD_ISSET(i, &rfds) || FD_ISSET(i, &wfds))
            {
                std::cout << i;
                if (this->serverS.count(i))
                    std::cout << " -> server socket (port " << this->serverS[i] << ")" << std::endl;
                else
                    std::cout << " -> client socket (" << (FD_ISSET(i, &rfds) ? "reading)": "writing)") << std::endl;
            }
        }
        std::cout << std::endl << "Waiting on select()..." << std::endl << std::endl;
        rc = select(_maxSd + 1, &rtmp, &wtmp, NULL, &timeout);
        if (rc < 0){
            std::cerr << "select() failed. End program" << std::endl;
            strerror(errno);
            break;
        }
        if (rc == 0)
        {
            if (!this->_timeout)
            {
                this->_timeout = true; // + action
                std::cout << "Timeout !" << std::endl;
            }
        }
        else
            this->_timeout = false;
        for (int i = 0 ; i <= _maxSd ; i++)
        {
            if (FD_ISSET(i, &rtmp) && this->serverS.count(i))
                newConnHandling(i);
            else
                existingConnHandling(i);
        }
        std::cout << "______________________________" << std::endl << std::endl;
    }
    for (int i = 0 ; i <= _maxSd ; i++)
    {
        if (FD_ISSET(i, &rfds))
            close(i);
    }
    return (1);
}

void Webserv::newConnHandling(int serverSd)
{
    int newSd;

    newSd = accept(serverSd, NULL, NULL);
    if (newSd <= 0)
    {
        strerror(errno);
        _endServ = true;
        return ;
    }
    std::cout << "New incoming connection " << newSd << std::endl;
    FD_SET(newSd, &rfds);
    if (newSd > _maxSd)
        _maxSd = newSd;
}

void Webserv::closeConn(int currSd)
{
    std::cout << "Connection " << currSd << " closed" << std::endl;
    close(currSd);
    FD_CLR(currSd, &rfds);
    delete clientS[currSd].first;
    delete clientS[currSd].second;
    clientS[currSd].first = NULL;
    clientS[currSd].second = NULL;
    if (currSd == _maxSd)
    {
        while (FD_ISSET(_maxSd, &rfds) == false)
            _maxSd -= 1;
    }
}

void Webserv::existingConnHandling(int currSd)
{
    if (FD_ISSET(currSd, &rtmp))
    {
        std::cout << "Socket client " << currSd << " has data to be red" << std::endl;
        if (receiveRequest(currSd) == 0)
            closeConn(currSd);
    }
    else if (FD_ISSET(currSd, &wtmp) && clientS.count(currSd))
    {
        std::cout << "Socket client " << currSd << " has data to be sent" << std::endl;
        sendResponse(currSd);
    }
}

int recving(int currSd, std::string *req)
{
    char bf[BUFFER_SIZE + 1];
    int rc = recv(currSd, bf, BUFFER_SIZE, 0);
    if (rc <= 0)
    {
        if (rc == -1)
            strerror(errno);
        return rc;
    }
    bf[rc] = 0;
    *req += std::string(bf);
    return rc;
}

int Webserv::receiveRequest(int currSd)
{
    int allbytes = 0, rc = BUFFER_SIZE;
    std::string req = "";

    std::cout << "Receiving . . ." << std::endl;
    while (rc == BUFFER_SIZE)
    {
        rc = recving(currSd, &req);
        if (rc <= 0)
        {
            if (rc == -1)
                strerror(errno);
            return rc;
        }
        allbytes += rc;
    }
    std::cout << "All data received : " << allbytes << " bytes" << std::endl;

    clientS[currSd] = std::make_pair(new Request(req), new Response());
    FD_CLR(currSd, &rfds);
    FD_SET(currSd, &wfds);
    return 1;
}

Response*	Webserv::handle_request(Parsing *config, Request *req) {
	std::string method = req->getValue("Method");
	Response* rep;
	if (method == "GET")
		rep = new Get(config, req);
	else if (method == "POST")
		rep = new Post(config, req);
	else if (method == "DELETE")
		rep = new Delete(config, req);
	else {
		std::cout << "Unknown method" << std::endl;
		return (NULL);
	}
    return rep;
}


void Webserv::sendResponse(int currSd)
{
    std::cout << "Sending . . ." << std::endl;
    clientS[currSd].second = handle_request(_config, clientS[currSd].first);
    int rc = send(currSd, (clientS[currSd].second->getResponse()).c_str(), (clientS[currSd].second->getResponse()).size(), 0);
    std::cout << "Response sent for " << clientS[currSd].first->getValue("File") << std::endl;
    if (rc < 0)
        strerror(errno); //a modifier pour retourner erreur si il ya
    FD_CLR(currSd, &wfds);
    delete clientS[currSd].first;
    delete clientS[currSd].second;
    clientS[currSd].first = NULL;
    clientS[currSd].second = NULL;
    FD_SET(currSd, &rfds);
}
