#pragma once
#include<pthread.h>
#include <sys/socket.h>
#include <queue>
#include "payload.h"

class Worker {
    int ID;
    int sockfd;
    char* buf;
    Payload* pl;
    pthread_t w_thread;
    std::queue<Payload *> Q;

    public:
        pthread_mutex_t *lock;
        pthread_cond_t *cv;
        Worker(int, int, pthread_mutex_t*, pthread_cond_t*);
        void push(Payload *);
        int work();
        int join();
        int init();
};

static void* work_wrapper(void *);