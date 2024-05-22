#pragma once
#include<pthread.h>
#include <sys/socket.h>
#include <queue>
#include "worker.h"
#include "payload.h"

class Scheduler {
    pthread_t w_thread;
    Worker** workers;
    int n_workers;
    std::queue<Payload *> Q;
    int curidx;

    public:
        Scheduler(Worker**, int);
        void push(Payload *);
        int init();
        void main_loop();
};

static void* schedule_wrapper(void *);