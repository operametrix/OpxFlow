#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>

#include "server.h"
#include "packets/packets.h"
#include "log/log.h"

#define MAX_EVENTS 1000

void setnonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int server_create_listener(struct in_addr address, uint16_t port)
{
    struct sockaddr_in servaddr;
    int fd = 0;
    int retval = 0;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if( fd == -1 )
    {
        LOG_CRITICAL("socket creation failed...");
        exit(EXIT_FAILURE);
    }

    /* Force TCP to don't wait TIME_WAIT after closing the socket */
    if( setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0 )
    {
        LOG_ERROR("setsockopt(SO_REUSEADDR) failed");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = address;
    servaddr.sin_port = port;

    retval = bind(fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if( retval != 0 )
    {
        LOG_CRITICAL("socket bind failed...");
        exit(EXIT_FAILURE);
    }

    retval = listen(fd, 100);
    if( retval != 0 )
    {
        LOG_CRITICAL("listen failed...");
        exit(EXIT_FAILURE);
    }

    LOG_INFO("server listening on %s:%d", inet_ntoa(servaddr.sin_addr), (int) ntohs(servaddr.sin_port));

    return fd;
}

int server_handle_connection(int connection_fd)
{
    return packets_read(connection_fd);
}

void server_run(int listener_fd)
{
    bool server_run = true;
    int connection_fd = 0, nfds, n;
    struct sockaddr_in addr;
    socklen_t addrlen;
    struct epoll_event ev, events[MAX_EVENTS];
    int epollfd;

    epollfd = epoll_create1(0);
    if (epollfd == -1)
    {
        LOG_CRITICAL("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listener_fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listener_fd, &ev) == -1)
    {
        LOG_CRITICAL("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    while(server_run)
    {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            LOG_CRITICAL("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for(n = 0; n < nfds; ++n)
        {
            if( events[n].data.fd == listener_fd )
            {
                addrlen = (socklen_t) sizeof(struct sockaddr);
                connection_fd = accept(listener_fd, (struct sockaddr *) &addr, &addrlen);

                if( connection_fd == -1 )
                {
                    LOG_CRITICAL("server acccept failed...");
                    exit(EXIT_FAILURE);
                }

                setnonblocking(connection_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = connection_fd;

                if( epoll_ctl(epollfd, EPOLL_CTL_ADD, connection_fd, &ev) == -1 )
                {
                    LOG_CRITICAL("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }

                LOG_INFO("new   connection from %s:%d", inet_ntoa(addr.sin_addr), (int) ntohs(addr.sin_port));
            }
            else
            {
                if( server_handle_connection(events[n].data.fd) == 0 )
                {
                    LOG_INFO("close connection from %s:%d", inet_ntoa(addr.sin_addr), (int) ntohs(addr.sin_port));

                    if( epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL) == -1 )
                    {
                        LOG_CRITICAL("epoll_ctl: conn_sock");
                        exit(EXIT_FAILURE);
                    }

                    close(events[n].data.fd);
                }
            }
        }
    }

    close(listener_fd);
    return;
}
