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

# define SERVER_PORT 8080
# define BUFFER_SIZE 10000

void clientRequest(char *buffer, int len)
{
    std::string request(buffer);
    std::cout << "<<<<< CLIENT REQUEST >>>>>" << std::endl << std::endl;
    for (int i = 0 ; i < request.size() ; i++)
        std::cout << request;
    std::cout << std::endl << std::endl;
}

std::string generateResponse(const std::string& htmlContent) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << htmlContent.length() << "\r\n";
    response << "\r\n";
    response << htmlContent;
    return (response.str());
}

std::string answerFormat()
{
    std::string htmlFilePath = "../html/data/browser.html";
    std::ifstream ifs(htmlFilePath);
    std::stringstream iss;
    iss << ifs.rdbuf();
    std::string htmlContent = iss.str();
    return (generateResponse(htmlContent));
}


int main()
{
    // int i, len, listen_sd, rc, max_sd, desc_ready, close_conn, new_sd;
    // char bufferR[BUFFER_SIZE];
    // char bufferS[BUFFER_SIZE];
    // int on = 1;
    // int end_server = false;

    // fd_set master_set, working_set;
    // sockaddr_in6 addr;
    // timeval timeout;
    
    // listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
    // if (listen_sd < 0)
    //     return (std::cerr << "listen_sd() failed" << std::endl, -1);

    // rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    // if (rc < 0)
    //     return (std::cerr << "setsockopt() failed" << std::endl, close(listen_sd), -1);
    // rc = ioctl(listen_sd, FIONBIO, (char*)&on);
    // if (rc < 0)
    //     return (std::cerr << "ioctl() failed" << std::endl, close(listen_sd), -1);

    // std::memset(&addr, 0, sizeof(addr));
    // addr.sin6_family = AF_INET6;
    // memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    // addr.sin6_port = htons(SERVER_PORT);
    // rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
    // if (rc < 0)
    //     return (std::cerr << "bind() failed" << std::endl, close(listen_sd), -1);

    // rc = listen(listen_sd, 32);
    // if (rc < 0)
    //     return (std::cerr << "listen() failed" << std::endl, close(listen_sd), -1);

    // FD_ZERO(&master_set);
    // max_sd = listen_sd;
    // // ajoute le fd listen_sd a master_set (= notre ensemble de soket a surveiller par select())
    // FD_SET(listen_sd, &master_set);

    // timeout.tv_sec = 3 * 60;
    // timeout.tv_usec = 0;
    
    while (end_server == false)
    {
        // A chaque iteration : working_set <= master_set ( = toutes les sockets sous surveillance)
        memcpy(&working_set, &master_set, sizeof(master_set));
        std::cout << "Waiting on select()..." << std::endl;
        // en attente qu'au moins une socket soit prete pour une operation de lecture, retourne nb socket pretes
        rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);
        // si rc < 0 : error, rc == 0 : timeout, dans les 2 cas on sort
        if (rc <= 0)
        {
            std::cerr << (rc < 0 ? "select() failed" : "select() timed out. End program") << std::endl;
            break;
        }
        desc_ready = rc;
        // tant qu'on a pas parcouru toutes les sockets surveillees (de 0 a max_sd)
        // && tant qu'il reste des sockets actives non traitees
        for (i = 0 ; i <= max_sd && desc_ready > 0 ; ++i)
        {
            // on check via ce fd si la socket est prete ou non
            if (FD_ISSET(i, &working_set))
            {
                // si oui on va la traiter donc on decremente le nb de socket qu'il reste a traiter
                desc_ready -= 1;
                // si la socket = socket d'ecoute (serveur) -> on va accepter une nouvelle connexion entrante
                if (i == listen_sd)
                {
                    std::cout << "Listening socket is readable" << std::endl;
                    new_sd = 0;
                    // tant qu'il reste des nouvelles connexions en attente d'etre acceptees
                    while (new_sd != -1)
                    {
                        // accepte nouvelle connexion entrante, cree socket client et retourne son sd
                        new_sd = accept(listen_sd, NULL, NULL);
                        if (new_sd < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                std::cerr << "accept() failed" << std::endl;
                                end_server = true;
                            }
                            break;
                        }
                        std::cout << "New incoming connection " << new_sd << std::endl;
                        // on ajoute ce sd a notre ensemble de socket a surveiller
                        FD_SET(new_sd, &master_set);
                        if (new_sd > max_sd)
                            max_sd = new_sd;
                    }
                }
                // si la socket = socket client -> on va taiter les donnees recues sur la socket
                else
                {
                    std::cout << "Descriptor " << i << " is readable" << std::endl;
                    close_conn = false;
                    // tant qu'il y a des donnees a recevoir
                    while (true)
                    {
                        // recoit des donnees de la socket i et les place dans buffer, renvoie nb bytes recues
                        rc = recv(i, bufferR, sizeof(bufferR), 0);
                        if (rc < 0)
                        {
                            // erreur ou en attente de reception des donnees -> boucle interrompue
                            if (errno != EWOULDBLOCK)
                            {
                                std::cerr << "recv() failed" << std::endl;
                                close_conn = true;
                            }
                            break;
                        }
                        // connexion fermee par le client -> boucle interrompue
                        if (rc == 0)
                        {
                            std::cerr << "Connection closed" << std::endl;
                            close_conn = true;
                            break;
                        }
                        clientRequest(bufferR, rc);
                        std::string response = answerFormat();
                        std::cout << len << " bytes received" << std::endl;
                        // renvoie ces memes donnees au client
                        rc = send(i, response.c_str(), response.size(), 0);
                        if (rc < 0)
                        {
                            std::cerr << "send() failed" << std::endl;
                            close_conn = true;
                            break;
                        }
                    }
                    // si la connexion doit etre fermee : on ferme le sd + on enleve le sd des sockets a surveiller
                    if (close_conn)
                    {
                        close(i);
                        FD_CLR(i, &master_set);
                        if (i == max_sd)
                        {
                            // si ct le fd le plus grand on recherche le fd le plus grand restant
                            while (FD_ISSET(max_sd, &master_set) == false)
                                max_sd -= 1;
                        }
                    }
                }
            }
        }
    }
    // on ferme toutes les sockets 
    for (i = 0 ; i <= max_sd ; i++)
    {
        if (FD_ISSET(i, &master_set))
            close(i);
    }
}