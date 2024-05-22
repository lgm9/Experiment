#pragma once
#include <sys/socket.h>

class Payload {
    int len;

    public:
        char* buf;
        struct sockaddr_in addr;
        Payload(int, char*, struct sockaddr_in);
};