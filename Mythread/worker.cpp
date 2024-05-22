#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "worker.h"
#include "payload.h"
#define BUF_SIZE 128

Worker::Worker(int n_ID, int fd) {
    ID = n_ID;
    sockfd = fd;
}

int Worker::work() {
    while(1) {
        if(!Q.empty()) {
            pl = Q.front();
            Q.pop();
            if((pl -> buf)[0] == 'S') {
                buf[0] = 'O';
                buf[1] = 'K';
                buf[2] = 0;
                sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&(pl -> addr), sizeof(pl -> addr));
            }
            else if((pl -> buf)[0] == 'G'){
                buf[0] = 'Y';
                buf[1] = 'E';
                buf[2] = 'T';
                buf[3] = 0;
                sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&(pl -> addr), sizeof(pl -> addr));
            }
            delete(pl);
        }
    }
    return 0;
}

void Worker::push(Payload *pl) {
    Q.push(pl);
}

int Worker::init() {
    buf = (char *)malloc(BUF_SIZE * sizeof(char));
    if(pthread_create(&w_thread, NULL, &work_wrapper, this) < 0) {
        printf("Thread not created\n");
    }
    return 0;
}

static void* work_wrapper(void *arg) {
    reinterpret_cast<Worker *>(arg) -> work();
    return 0;
}