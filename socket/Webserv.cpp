#include "Webserv.hpp"

Webserv::Webserv() : _on(1), _endServ(false), _serverPort(8080), _run(1)
{
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

bool Webserv::init()
{
   _listenSd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenSd < 0)
        return (std::cerr << "socket() failed" << std::endl, -1);

    _returnCode = setsockopt(_listenSd, SOL_SOCKET, SO_REUSEADDR, (char*)&_on, sizeof(_on));
    if (_returnCode < 0)
        return (std::cerr << "setsockopt() failed" << std::endl, close(_listenSd), -1);
    _returnCode = ioctl(_listenSd, FIONBIO, (char*)&_on);
    if (_returnCode < 0)
        return (std::cerr << "ioctl() failed" << std::endl, close(_listenSd), -1);

    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin6_family = AF_INET6;
    memcpy(&_addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    _addr.sin6_port = htons(_serverPort);
    _returnCode = bind(_listenSd, (struct sockaddr *)&_addr, sizeof(_addr));
    if (_returnCode < 0)
        return (std::cerr << "bind() failed" << std::endl, close(_listenSd), -1);

    _returnCode = listen(_listenSd, 32);
    if (_returnCode < 0)
        return (std::cerr << "listen() failed" << std::endl, close(_listenSd), -1);

    FD_ZERO(&_masterSet);
    _maxSd = _listenSd;
    // ajoute le fd _listenSd a _masterSet (= notre ensemble de soket a surveiller par select())
    FD_SET(_listenSd, &_masterSet);
    return (EXIT_SUCCESS);
}

bool Webserv::process()
{
    while (!_endServ)
    {
        memcpy(&_workingSet, &_masterSet, sizeof(_masterSet));
        std::cout << "Waiting on select()..." << std::endl;
        _returnCode = select(_maxSd + 1, &_workingSet, NULL, NULL, &_timeOut);
        if (_returnCode <= 0){
            std::cerr << (_returnCode < 0 ? "select() failed" : "select() timed out. End program") << std::endl;
            break;
        }
        _descReady = _returnCode;
        for (int i = 0 ; i <= _maxSd && _descReady > 0 ; ++i)
        {
            if (FD_ISSET(i, &_workingSet))
            {
                _descReady -= 1;
                if (i == _listenSd)
                    newConnHandling();
                else
                    existingConnHandling(i);
            }
        }  
        for (int i = 0 ; i <= _maxSd ; i++)
        {
            if (FD_ISSET(i, &_masterSet))
                close(i);
        }
    }
    return (EXIT_SUCCESS);
}

void Webserv::newConnHandling()
{
    std::cout << "Listening socket is readable" << std::endl;
    _newSd = 0;
    while (_newSd != -1)
    {
        _newSd = accept(_listenSd, NULL, NULL);
        if (_newSd < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                std::cerr << "accept() failed" << std::endl;
                _endServ = true;
            }
            break ;
        }
        std::cout << "New incoming connection " << _newSd << std::endl;
        FD_SET(_newSd, &_masterSet);
        if (_newSd > _maxSd)
            _maxSd = _newSd;
    }
}
void Webserv::closeConn(int currSd)
{
    close(currSd);
    FD_CLR(currSd, &_masterSet);
    if (currSd == _maxSd)
    {
        while (FD_ISSET(_maxSd, &_masterSet) == false)
            _maxSd --;
    }
}

void Webserv::existingConnHandling(int currSd)
{
    int run = 1;
    std::cout << "Descriptor " << currSd << " is readable" << std::endl;
    _closeConn = false;
    while (_run)
    {
        receiveRequest(currSd);
        sendResponse(currSd);
        if (_closeConn)
            closeConn(currSd);
    }
}

void Webserv::handlingError()
{
    if (_returnCode < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            std::cerr << "recv() or send() failed" << std::endl;
            _closeConn = true;
        }
        _run = 0;
        return ;
    }
    if (_returnCode == 0)
    {
        std::cerr << "Connection closed" << std::endl;
        _closeConn = true;
        _run = 0;
    }
}

void Webserv::receiveRequest(int currSd)
{
    _returnCode = recv(currSd, _request.bufferRequest, sizeof( _request.bufferRequest), 0);
    if (_returnCode <= 0)
        return (handlingError());
    _request.parsingFormat();
}

void Webserv::sendResponse(int currSd)
{
    _response.generate();
    _returnCode = send(currSd, _response._response.c_str(), _response._contentLength, 0);
    if (_returnCode < 0)
        handlingError();
}
