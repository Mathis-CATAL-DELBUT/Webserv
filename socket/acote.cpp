#include "Webserv.hpp"

Webserv::Webserv() : _endServ(false), _serverPort(8080), _run(1)
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
    _returnCode = ioctl(_listenSd, FIONBIO, (char*)&on);
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

    FD_ZERO(&_masterSet);
    _maxSd = _listenSd;
    FD_SET(_listenSd, &_masterSet);
    return (1);
}

bool Webserv::process()
{
    while (!_endServ)
    {
        memcpy(&_workingSet, &_masterSet, sizeof(_masterSet));
        // std::cout << "Waiting on select()..." << std::endl;
        _returnCode = select(_maxSd + 1, &_workingSet, NULL, NULL, &_timeOut);
        if (_returnCode <= 0){
            std::cerr << (_returnCode < 0 ? "select() failed" : "select() timed out. End program") << std::endl;
            perror("select");
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
    }
    for (int i = 0 ; i <= _maxSd ; i++)
    {
        if (FD_ISSET(i, &_masterSet))
            close(i);
    }
    return (1);
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
    std::cout << "closing ! " << std::endl;
    close(currSd);
    FD_CLR(currSd, &_masterSet);
    if (currSd == _maxSd)
    {
        while (FD_ISSET(_maxSd, &_masterSet) == false)
            _maxSd -= 1;
    }
}

void Webserv::existingConnHandling(int currSd)
{
    // int run = 1;
    // std::cout << "Descriptor " << currSd << " is readable" << std::endl;
    _closeConn = false;
    while (true)
    {
        if (receiveRequest(currSd) == 0)
            break;
        if (sendResponse(currSd) == 0)
            break;
        if (_closeConn)
            closeConn(currSd);
    }
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
        // _run = 0;
        return 0;
    }
    if (_returnCode == 0)
    {
        // std::cerr << "Connection closed" << std::endl;
        _closeConn = true;
        // _run = 0;
        return 0;
    }
    return 0;
}

int Webserv::receiveRequest(int currSd)
{
    char bf[BUFFER_SIZE];
    _returnCode = recv(currSd, bf, sizeof( bf), 0);
    if (_returnCode <= 0)
        return (handlingErrorConn());
    _request.requestTreatment(bf);
    // _request.print();
    return 1;
}

// void Webserv::sendResponse(int currSd)
// {
//     _response.generate(_request);
//     _returnCode = send(currSd, _response.getResponse().c_str(), _response.getContentLength(), 0);
//     if (_returnCode < 0)
//         handlingError();
// }

int Webserv::sendResponse(int currSd)
{
    std::string path;
    std::string contentType;
    path = "../html/";
    std::cout << "EXTENSION = " << _request.getExtension() << std::endl;
    if (_request.getExtension() == "webp" || _request.getExtension() == "jpg")
        path += "image/";
    else
        path += "data/";
    std::cout << "ressource = " << _request.getRessource() << std::endl;
    path += _request.getRessource();
    std::cout << "PATH = " << path << std::endl;
    std::ifstream ifs(path.c_str());
    std::stringstream iss;
    iss << ifs.rdbuf();
    std::string content = iss.str();
    std::string response = formating(content);
    // std::cout << response << std::endl;
    int rc = send(currSd, response.c_str(), response.size(), 0);
    if (rc < 0)
        return (handlingErrorConn());
    return 1;
}

std::string Webserv::formating(std::string content) 
{
    std::ostringstream response;
    std::unordered_map<std::string, std::string> map{
        {"webp", "image"},
        {"jpeg", "image"},
        {"html", "text"},
        {"javascript", "application"},
        {"css", "text"}
    };
    std::string type;
    type = (this->_request.getExtension() == "js" ? "javascript" : (this->_request.getExtension() == "jpg" ? "jpeg" : this->_request.getExtension()));
    response << _request.getProtocol() << " 200 OK\r\n";
    std::cout << _request.getProtocol() << " 200 OK\r\n";
    response << "Content-Type: " << map[type] << "/" << type << "\r\n";
    std::cout << "Content-Type: " << map[type] << "/" << type << "\r\n";
    response << "Content-Length: " << content.length() << "\r\n";
    std::cout << "Content-Length: " << content.length() << "\r\n";
    response << "\r\n";
    response << content;
    return (response.str());
}

//      for (int i = 0; i <= _max_fd; ++i) {
        //     if (FD_ISSET(i, &working_set_recv) && i == 0) {
        //         if (userExit())
        //             return(false);
        //     }
        //     else if (FD_ISSET(i, &working_set_recv) && isServerSocket(i)) 
        //         acceptNewCnx(i);
        //     else if (FD_ISSET(i, &working_set_recv) && _requests.count(i))
        //         readRequest(i, *_requests[i]);
        //     else if (FD_ISSET(i, &working_set_write) && _requests.count(i))
        //         sendResponse(i, *_requests[i]); ->to analyse
        // }