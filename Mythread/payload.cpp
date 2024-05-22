#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "payload.h"
#define BUF_SIZE 128

Payload::Payload(int l, char* inbuf, struct sockaddr_in inaddr) {
    addr = inaddr;
    buf = (char *)malloc(sizeof(char) * BUF_SIZE);
    len = l;
    for(int i = 0 ; i < l ; i++) buf[i] = inbuf[i];
}