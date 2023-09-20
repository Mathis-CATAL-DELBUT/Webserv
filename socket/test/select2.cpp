#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define SERVER_PORT  12345

#define TRUE             1
#define FALSE            0


// Fonction pour générer une réponse HTTP à partir d'un contenu HTML
std::string generateResponse(const std::string& htmlContent) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << htmlContent.length() << "\r\n";
    response << "\r\n";
    response << htmlContent;
    return response.str();
}

int main ()
{
   int    len, rc, on = 1;
   int    listen_sd, max_sd, new_sd;
   int    desc_ready, end_server = FALSE;
   int    close_conn;
   char   buffer[10024];
   sockaddr_in6   addr;
   timeval       timeout;
   fd_set fds, rfds;
   listen_sd = socket(AF_INET, SOCK_STREAM, 0);
   rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
                   (char *)&on, sizeof(on));
   rc = ioctl(listen_sd, FIONBIO, (char *)&on);
   memset(&addr, 0, sizeof(addr));
   addr.sin6_family      = AF_INET;
   memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
   addr.sin6_port        = htons(SERVER_PORT);
   rc = bind(listen_sd,(struct sockaddr *)&addr, sizeof(addr));
   rc = listen(listen_sd, 32);
   max_sd = listen_sd;
   std::cout << "listen sd = " << listen_sd << std::endl;
   FD_ZERO(&fds);
   FD_SET(listen_sd, &fds);
   timeout.tv_sec  = 3 * 60;
   timeout.tv_usec = 0;
   while (1)
   {
      printf("Waiting on select()...\n");
      rfds = fds;
      rc = select(max_sd + 1, &rfds, NULL, NULL, &timeout);
      if (rc < 0) {
            perror("select() failed");
            exit(EXIT_FAILURE);
        }

      for (int fd = 0; fd <= max_sd ; fd++)
      {
        std::cout << fd << std::endl;
            if (FD_ISSET(fd, &rfds))
            {
                if (fd == listen_sd)
                {
                    new_sd = accept(listen_sd, NULL, NULL);
                    FD_SET(new_sd, &fds);
                    if (new_sd > max_sd)
                        max_sd = new_sd;
                    std::cout << "New connection" << std::endl;
                }
                else
                {
                    printf("Reading...\n");
                    rc = recv(fd, buffer, sizeof(buffer), 0);
                    std::cout << "<<<<< REQUEST CLIENT >>>>>" << std::endl;
                    std::cout << buffer << std::endl << std::endl;
                    if (rc == -1)
                        std::cout << "error" << std::endl;
                    else if (rc == 0)
                    {
                        std::cout << "Connection closed" << std::endl;
                        close(fd);
                        FD_CLR(fd, &fds);
                    }
                    else
                    {
                        std::cout << "Writing..." << std::endl;
                        std::string htmlFilePath = "index.html";
                        std::ifstream htmlFile(htmlFilePath);
                        std::stringstream htmlContentStream;
                        htmlContentStream << htmlFile.rdbuf();
                        std::string htmlContent = htmlContentStream.str();
                        std::string response = generateResponse(htmlContent);
                        std::cout << "<<<<< RESPONSE FROM SERVER SENT >>>>>" << std::endl;
                        // std::cout << response << std::endl;
                        rc = send(fd, response.c_str(), response.length(), MSG_DONTWAIT);
                    }
                }
            }
        }
    }
}