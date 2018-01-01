/* 
 * created by frank
 * 2018.01.01
 */
 
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_QUEUE_LENGTH 32
#define	MAX(x, y)	((x) > (y) ? (x) : (y))

// these functions is provided by teacher
int errexit(const char *format, ...);
int passiveTCP(const char *port, int qlen);

int main(int argc, char *argv[])
{
    int max_fd, current_fd;
    fd_set fds;
    FD_ZERO(&fds);
    
    // base port number
    // default is 1200
    char *basePort = "1200";
    switch (argc) {
        case 1:
            break;
        case 2:
            basePort = argv[1];
            break;
        default:
            exit(0);
            break;
    }
    
    // calculate the port of reserve
    int i_port = atoi(basePort);
    char reserve_port[10];
    sprintf(reserve_port, "%d", i_port + 1);
    
    // get two sockets of two services
    int sum_socket = passiveTCP(basePort, MAX_QUEUE_LENGTH);
    int reserve_socket = passiveTCP(reserve_port, MAX_QUEUE_LENGTH);
    FD_SET(sum_socket, &fds);
    FD_SET(reserve_socket, &fds);
    
    max_fd = (MAX(sum_socket, reserve_socket)) + 1;
    while(1) {
        // waif for readable sockfd
        if(select(max_fd, &fds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0) {
            if(errno == EINTR)
                continue;
            errexit("select error: %s\n", strerror(errno));
        }
        // look for request
        for(current_fd = 0; current_fd < max_fd; current_fd++) {
            if (FD_ISSET(current_fd, &fds)) {
                printf("%d", current_fd);
            }
        }
    }
}

int passiveTCP(const char *port, int qlen)
{
    int s;
    
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    // convert host byte order to network byte order
    sin.sin_port = htons(atoi(port));  
    
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s < 0)  // something error
        errexit("can't create socket: %s\n", strerror(errno));
        
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        errexit("can't bind to %s port: %s\n", port, strerror(errno));
    
    if (listen(s, qlen) < 0)
        errexit("can't listen on %s port: %s\n", port, strerror(errno));
    return s;
}
