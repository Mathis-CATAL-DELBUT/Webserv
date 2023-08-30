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

# define SERVER_PORT 8080

int main()
{
    int i, len, listen_sd, rc, max_sd, desc_ready, close_conn, new_sd;
    char buffer[80];
    int on = 1;
    int end_server = false;

    struct fd_set master_set, working_set;
    struct timeval timeout;
    
    listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_sd < 0)
        return (std::cerr << "listen_sd() failed" << std::endl, -1);
    rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    if (rc < 0)
        return (std::cerr << "setsockopt() failed" << std::endl, close(listen_sd), -1);
    rc = ioctl(listen_sd, FIONBIO, (char*)&on);
    if (rc < 0)
        return (std::cerr << "ioctl() failed" << std::endl, close(listen_sd), -1);
    rc = listen(listen_sd, 32);
    if (rc < 0)
        return (std::cerr << "listen() failed" << std::endl, close(listen_sd), -1);

    FD_ZERO(&master_set);
    max_sd = listen_sd;
    FD_SET(listen_sd, &master_set);

    timeout.tv_sec = 3 * 60;
    timeout.tv_usec = 0;
    
    while (end_server == false)
    {
        memcpy(&working_set, &master_set, sizeof(master_set));
        std::cout << "Waiting on select()..." << std::endl;
        rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);
        if (rc <= 0)
        {
            std::cerr << (rc < 0 ? "select() failed" : "select() timed out. End program") << std::endl;
            break;
        }
        desc_ready = rc;
        for (i = 0 ; i <= max_sd && desc_ready > 0 ; ++i)
        {
            if (FD_ISSET(i, &working_set))
            {
                desc_ready -= 1;
                if (i == listen_sd)
                {
                    std::cout << "Listening socket is readable" << std::endl;
                    new_sd = 0;
                    while (new_sd != -1)
                    {
                        new_sd = accept(listen_sd, NULL, NULL);
                        if (new_sd < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                std::cerr << "accept() failed" << std::endl;
                                end_server = TRUE;
                            }
                            break;
                        }
                        std::cout << "New incoming connection " << new_sd << std::endl;
                        FD_SET(new_sd, &master_set);
                        if (new_sd > max_sd)
                            max_sd = new_sd;
                    }
                }
                else
                {
                    std::cout << "Descriptor " << i << " is readable" << std:endl;
                    close_conn = FALSE;
                    while (true)
                    {
                        rc = recv(i, buffer, sizeof(buffer), 0);
                        if (rc < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                std::cerr << "recv() failed" << std::endl;
                                close_conn = true;
                            }
                            break;
                        }
                        if (rc == 0)
                        {
                            std::cerr << "Connection closed" << std::endl;
                            close_conn = true;
                            break;
                        }
                        len = rc;
                        std::cout << len << " bytes received" << std::endl;
                        rc = send(i, buffer, len, 0);
                        if (rc < 0)
                        {
                            std::cerr << "send() failed" << std::endl;
                            close_conn = true;
                            break;
                        }
                    }
                    if (close_conn)
                    {
                        close(i);
                        FD_CLR(i, &master_set);
                        if (i == max_sd)
                        {
                            while (FD_ISSET(max_sd, &master_set) == false)
                                max_sd -= 1;
                        }
                    }
                }
            }
        }
    }
    for (i = 0 ; i <= max_sd ; i++)
    {
        if (FD_ISSET(i, &master_set))
            close(i);
    }
}