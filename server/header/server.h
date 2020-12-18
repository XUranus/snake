//
// Created by xuranus on 9/21/18.
//

#ifndef SNAKE_SERVER_H
#define SNAKE_SERVER_H

#include<cstdio>
#include<cstring>
#include<ctime>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include<sys/stat.h>
#include<fcntl.h>

#include<vector>

#include "zone.h"

struct server {
    int server_socket;
    struct sockaddr_in server_addr;

    int client_num ;
    std::vector<int> client_socket;
    struct timeval timeout;
    fd_set client_fdset;

    zone* battle_zone;

    bool active;
    long long slot,frame;

    server(int,int,int, int);//port(room_id) ,client_num, zone_row, zone_col
    bool start();
    void stop();

    void send_sync_data();//ALARM sub hander
    void fetch_data();
    ~server();
};


#endif //SNAKE_SERVER_H
