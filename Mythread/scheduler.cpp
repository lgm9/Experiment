#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "scheduler.h"
#define BUF_SIZE 128

Scheduler::Scheduler(Worker** ws, int n_w) {
    workers = ws;
    n_workers = n_w;
}

void Scheduler::main_loop() {
    while(1) {
        if(!Q.empty()) {
            workers[curidx] -> push(Q.front());
            Q.pop();
            curidx = (curidx + 1) % n_workers;
        }
    }
}

void Scheduler::push(Payload* pl) {
    Q.push(pl);
}

int Scheduler::init() {
    curidx = 0;
    if(pthread_create(&w_thread, NULL, &schedule_wrapper, this) < 0) {
        printf("Thread not created\n");
    }
    return 0;
}

static void* schedule_wrapper(void *arg) {
    reinterpret_cast<Scheduler *>(arg) -> main_loop();
    return 0;
}