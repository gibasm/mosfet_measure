#include "tcp.h"
#include <complex.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>

#define PRINTF_FL() \
    fprintf(stderr, "%s:%d: ", __FILE__, __LINE__)

int tcp_connect(const char* ipaddr, int port)
{
    int fd;
    struct sockaddr_in addr; 

    assert(ipaddr);
    
    memset(&addr, 0, sizeof(struct sockaddr_in)); 
    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ipaddr);
    
    fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        PRINTF_FL();
        return -1;
    }

    if(connect(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1) {
        PRINTF_FL(); 
        return -1; 
    }

    return fd;
}
