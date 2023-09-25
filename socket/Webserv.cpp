#include "Webserv.hpp"

Webserv::Webserv() : _endServ(false), _serverPort(8080)
{
    _timeOut.tv_sec = 180;
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
    _returnCode = fcntl(_listenSd, F_SETFL, O_NONBLOCK);
    if (_returnCode < 0)
        return (handlingErrorInit("fcntl"));
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
    _endServ = false;
    FD_ZERO(&rfds);
    FD_SET(_listenSd, &rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&rtmp);
    FD_ZERO(&wtmp);
    return (1);
}
bool Webserv::process()
{
    while (!_endServ)
    {
        rtmp = rfds;
        wtmp = wfds;
        for (int i = 3 ; i <= _maxSd ; i++)
            std::cout << "Watched by select : " << i << (clientS.count(i) ? " -> client socket" : " -> server socket") << std::endl;
        std::cout << "Waiting on select()..." << std::endl << std::endl;
        _returnCode = select(_maxSd + 1, &rtmp, &wtmp, NULL, &_timeOut);
        if (_returnCode <= 0){
            std::cerr << (_returnCode < 0 ? "select() failed" : "select() timed out. End program") << std::endl;
            strerror(errno);
            break;
        }
        for (int i = 0 ; i <= _maxSd ; i++)
        {
            if (FD_ISSET(i, &rtmp) && i == _listenSd)
                newConnHandling();
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

void Webserv::newConnHandling()
{
   _newSd = accept(_listenSd, NULL, NULL);
    if (_newSd <= 0)
    {
        strerror(errno);
        _endServ = true;
        return ;
    }
    std::cout << "New incoming connection " << _newSd << std::endl;
    FD_SET(_newSd, &rfds);
    if (_newSd > _maxSd)
        _maxSd = _newSd;
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

int Webserv::handlingErrorConn()
{
    if (_returnCode == -1)
        strerror(errno);
    return _returnCode;
}

int Webserv::receiveRequest(int currSd)
{
    std::cout << "Receiving . . ." << std::endl;
    char bf[BUFFER_SIZE];
    _returnCode = recv(currSd, bf, sizeof( bf), 0);
    if (_returnCode <= 0)
    {
        if (_returnCode == -1)
            strerror(errno);
        return _returnCode;
    }
    bf[_returnCode] = 0;
    clientS[currSd] = std::make_pair(new Request(bf), new Response());
    FD_CLR(currSd, &rfds);
    FD_SET(currSd, &wfds);
    return 1;
}


void Webserv::sendResponse(int currSd)
{
    std::cout << "Sending . . ." << std::endl;
    (clientS[currSd].second)->generateResponse(*(clientS[currSd]).first);
    int rc = send(currSd, (clientS[currSd].second->getResponse()).c_str(), (clientS[currSd].second->getResponse()).size(), 0);
    std::cout << "Response sent for " << clientS[currSd].first->getRessource() << std::endl;
    if (rc < 0)
        strerror(errno);
    FD_CLR(currSd, &wfds);
    delete clientS[currSd].first;
    delete clientS[currSd].second;
    clientS[currSd].first = NULL;
    clientS[currSd].second = NULL;
    FD_SET(currSd, &rfds);
}
