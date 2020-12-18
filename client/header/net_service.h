//
// Created by xuranus on 9/21/18.
//

#ifndef SNAKE_CLIENT_NET_SERVICE_H
#define SNAKE_CLIENT_NET_SERVICE_H

#include <string>
#include "zone.h"


typedef struct net_service {
    struct sockaddr_in* server_addr;
    int server_socket;
    zone* _zone;
    int faild_times;

    net_service(const char*,int,zone*);
    ~net_service();
    int connect();
    void close();
    void send(std::string);
    void read();//block io


} net_service;


#endif //SNAKE_CLIENT_NET_SERVICE_H
