#include "Webserv.hpp"

Webserv::Webserv() : _endServ(false), _serverPort(8080), _run(1)
{
    _timeOut.tv_sec = 3 * 60;
    _timeOut.tv_usec = 0;
}

Webserv::Webserv(Parsing* config) : _endServ(false), _serverPort(8080), _run(1)
{
    _config = config;
    _timeOut.tv_sec = 3 * 60;
    _timeOut.tv_usec = 0;
}

Webserv::~Webserv() {}

Webserv::Webserv(Webserv const &copy) {}

Webserv &Webserv::operator=(Webserv const &rhs)
{
    if (this == &rhs)
        return (*this);
    return (*this);
}

int Webserv::handlingErrorInit(std::string function)
{
    std::cerr << function << "() failed" << std::endl;
    if (_listenSd > 0)
        close(_listenSd);
    return (0);
}

bool Webserv::init()
{
    sockaddr_in6 addr;
    int on = 1;

   _listenSd = socket(AF_INET6, SOCK_STREAM, 0);
    if (_listenSd < 0)
        return (handlingErrorInit("listen"));
    _returnCode = setsockopt(_listenSd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    if (_returnCode < 0)
        return (handlingErrorInit("setsockopt"));
    _returnCode = ioctl(_listenSd, FIONBIO, (char *)&on);
    if (_returnCode < 0)
        return (handlingErrorInit("ioctl"));

    std::memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port = htons(_serverPort);
    _returnCode = bind(_listenSd, (struct sockaddr *)&addr, sizeof(addr));
    if (_returnCode < 0)
        return (handlingErrorInit("bind"));

    _returnCode = listen(_listenSd, 32);
    if (_returnCode < 0)
        return (handlingErrorInit("listen"));

    _maxSd = _listenSd;
    FD_ZERO(&fds);
    FD_SET(_listenSd, &fds);
    return (1);
}

bool Webserv::process()
{
    while (1)
    {
        rfds = fds;
        std::cout << "Waiting on select()..." << std::endl;
        _returnCode = select(_maxSd + 1, &rfds, NULL, NULL, &_timeOut);
        if (_returnCode <= 0){
            std::cerr << (_returnCode < 0 ? "select() failed" : "select() timed out. End program") << std::endl;
            perror("select");
            break;
        }
        for (int i = 0 ; i <= _maxSd ; i++)
        {
            std::cout << i << std::endl;
            if (FD_ISSET(i, &rfds))
            {
                if (i == _listenSd)
                    newConnHandling();
                else
                    existingConnHandling(i);
            }
        }  
    }
    for (int i = 0 ; i <= _maxSd ; i++)
    {
        if (FD_ISSET(i, &fds))
            close(i);
    }
    return (1);
}

void Webserv::newConnHandling()
{
   _newSd = accept(_listenSd, NULL, NULL);
    if (_newSd <= 0)
    {
        if (errno != EWOULDBLOCK)
        {
            std::cerr << "accept() failed" << std::endl;
            _endServ = true;
        }
        return ;
    }
    std::cout << "New incoming connection " << _newSd << std::endl;
    FD_SET(_newSd, &fds);
    if (_newSd > _maxSd)
        _maxSd = _newSd;
}

void Webserv::closeConn(int currSd)
{
    std::cout << "closing ! " << std::endl;
    close(currSd);
    FD_CLR(currSd, &fds);
    if (currSd == _maxSd)
    {
        while (FD_ISSET(_maxSd, &fds) == false)
            _maxSd -= 1;
    }
}

void Webserv::existingConnHandling(int currSd)
{
   std::cout << "Descriptor " << currSd << " is readable" << std::endl;
    if (receiveRequest(currSd) != 0)
        sendResponse(currSd);
    else
        closeConn(currSd);
}

int Webserv::handlingErrorConn()
{
    if (_returnCode < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            std::cerr << "recv() or send() failed" << std::endl;
            _closeConn = true;
        }
        return _returnCode;
    }
    if (_returnCode == 0)
    {
        std::cerr << "Connection closed" << std::endl;
        _closeConn = true;
    }
    return _returnCode;
}

int Webserv::receiveRequest(int currSd)
{
    char bf[BUFFER_SIZE];
    _returnCode = recv(currSd, bf, sizeof( bf), 0);
    if (_returnCode <= 0)
        return (handlingErrorConn());
    bf[_returnCode] = 0;
    _request = new Request(std::string(bf));
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


int Webserv::sendResponse(int currSd)
{
    this->_response = handle_request(_config, _request);
    int rc = send(currSd, (this->_response->getResponse()).c_str(), (this->_response->getResponse()).size(), 0);
    std::cout << "SENT" << std::endl;
    if (rc < 0)
        return (handlingErrorConn());
    return 1;
}
