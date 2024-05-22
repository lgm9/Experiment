#include <cstdio>
#include <time.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <queue>
#include "worker.h"
#include "scheduler.h"
#include "rocksdb/db.h"
#include "payload.h"
#define BUF_SIZE 128

struct sockaddr_in serv_addr, cli_addr;
socklen_t addrlen = sizeof(cli_addr);
int server_fd, client_fd, rclen;
char* mainbuf;
Worker **workers;
Scheduler *main_scheduler;
int num_workers = 8;

int init_socket() {
    mainbuf = (char *)malloc(BUF_SIZE * sizeof(char));

    server_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(server_fd <= 0) {
        printf("Failed to initalize socket\n");
        return 1;
    }
    
    printf("Successfully initalized socket\n");
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("The number of workers should be given\n");
        return 0;
    }

    if(init_socket()) {
        return 0;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4000);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(-1 == bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
        printf("bind error\n");
        return 0;
    }

    workers = (Worker **)malloc(num_workers * sizeof(Worker *));
    for(int i = 0 ; i < num_workers ; i++) {
        workers[i] = new Worker(i, server_fd);
        workers[i] -> init(); 
    }

    main_scheduler = new Scheduler(workers, num_workers);
    main_scheduler -> init();

    while(1) {
        rclen = recvfrom(server_fd, mainbuf, BUF_SIZE, 0, (struct sockaddr*)&cli_addr, &addrlen);
        main_scheduler -> push(new Payload(rclen, mainbuf, cli_addr));
    }

    close(server_fd);

    return 0;
}