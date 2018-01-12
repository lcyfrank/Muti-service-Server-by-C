/* 
 * created by frank
 * 2018.01.01
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_QUEUE_LENGTH 32
#define	MAX(x, y)	((x) > (y) ? (x) : (y))

// this functions is provided by teacher
int errexit(const char *format, ...);

int passiveTCP(const char *port, int qlen);
void handleRequest(int fd);

static int service_fds[2];

int main(int argc, char *argv[])
{
    int max_fd, current_fd;
    fd_set fds;
    fd_set b_fds;  // backup
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
    
    service_fds[0] = sum_socket;
    service_fds[1] = reserve_socket;
    
    memcpy(&b_fds, &fds, sizeof(fds));
    
    max_fd = (MAX(sum_socket, reserve_socket)) + 1;
    while(1) {
        memcpy(&fds, &b_fds, sizeof(fds));
        // wait for readable sockfd
        if(select(max_fd, &fds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0) {
            if(errno == EINTR)
                continue;
            errexit("select error: %s\n", strerror(errno));
        }
        // look for request
        for(current_fd = 0; current_fd < max_fd; current_fd++) {
            if (FD_ISSET(current_fd, &fds)) {
                handleRequest(current_fd);
            }
        }
    }
}

void handleRequest(int fd)
{
    struct sockaddr_in fsin;
    unsigned int alen;
    alen = sizeof(fsin);
    
    char buffer[100];
    memset(buffer, 0, 100);
    
    int new_fd = accept(fd, (struct sockaddr *)&fsin, &alen);
    if (new_fd < 0)
        errexit("accept error: %s\n", strerror(errno));
    
    if (read(new_fd, buffer, 100) < 0)
        errexit("read error: %s\n", strerror(errno));
    
    int flag = fork();
    switch (flag)
    {
        case 0:  // parent
            return;
        case -1:
            errexit("fork error: %s\n", strerror(errno));
            break;
        default:
            break;
    }
    // child
    if (fd == service_fds[0])
        execl("/home/lincy/Desktop/Muti-service Server by C/services/Sum", "Sum", buffer, (char *)NULL);
    else 
        execl("/home/lincy/Desktop/Muti-service Server by C/services/Reverse", "Reverse", buffer, (char *)NULL);
    exit(0);
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
