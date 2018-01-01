/*
 * created by frank
 * 2018.01.01
 */

#include <sys/socket.h>
#include <netinet/in.h> 
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// this function is provided by teacher 
int	errexit(const char *format, ...);

int connectTCP(const char *host, const char *port);
 
int main(int argc, char *argv[])
{
    char *host = "0.0.0.0";
    char *port = "1200";
    switch (argc) {
        case 1:
            break;
        case 3:
            port = argv[2];
        case 2:
            host = argv[1];
            break;
        default:
            break;
    }
    
    int s = connectTCP(host, port);
    
    return 0;
}

int connectTCP(const char *host, const char *port)
{
    int s;
    struct hostent *phe;
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    
    
    phe = gethostbyname(host);
    
    sin.sin_family = AF_INET;
    memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    sin.sin_port = htons(atoi(port));
     
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s < 0)  // something error
        errexit("can't create socket: %s\n", strerror(errno));
    
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        errexit("can't connect to %s:%s\n", host, port);
        
    return s;
} 
