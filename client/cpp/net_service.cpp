//
// Created by xuranus on 9/21/18.
//

#include "../header/net_service.h"
#include "../header/logger.h"
#include "../header/gameMessage.pb.h"
#include "../header/zone.h"

#include<string>
#include<cstdio>
#include <string>
#include<cstring>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <curses.h>


net_service::net_service(const char* server_addr_str,int server_port,zone* z) {
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    server_addr = new struct sockaddr_in;
    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(server_port);
    server_addr->sin_addr.s_addr = inet_addr(server_addr_str);

    _zone = z;
    faild_times = 0;
}

net_service::~net_service() {
    delete server_addr;
}

int net_service::connect() {
    //client connect to server
    return ::connect(server_socket, (struct sockaddr*)server_addr, sizeof(*server_addr));
}

void net_service::close() {
    ::close(server_socket);
}

void net_service::send(std::string _request) {
    const char* request = _request.c_str();
    if(write(server_socket, request, sizeof(request))>0) {
        //printf("%s\n",request);
    } else {
        perror("fail send request");
    }
}

void net_service::read() {
    const int MAX_BUFFER = 5000;
    char response[5000];
    unsigned int n;
    if((n=(unsigned int)::recv(server_socket, response, MAX_BUFFER,0))>0){
        if(MAX_BUFFER < n) {
            perror("BUFFER LIMIT EXCEED");
            exit(1);
        }

        GameProto::ServerMessage msg;
        bool res =  msg.ParseFromArray(response,n);
        
        GameProto::BattleField battleField = msg.battle_field();
        // std::cout << msg.DebugString() << std::endl;

        _zone->clear();
        _zone->print_border('*',3);


        for(int i=0;i<battleField.snake_size();i++) { // print snake
            const GameProto::Snake& snake = battleField.snake(i);

            char score_str[30];
            sprintf(score_str,"snake[%d] scores %d",i+1,snake.node_size()*100);
            mvaddnstr(5+i,_zone->column*2+5,score_str,strlen(score_str));

            for(int j=0;j<snake.node_size();j++) {
                _zone->mvprintch(snake.node(j).x(),snake.node(j).y(),'(',')',i);
            }
        }

        for(int i=0;i<battleField.food_size();i++) { // print food
            _zone->mvprintch(battleField.food(i).x(),battleField.food(i).y(),'[',']',2);
        }
        faild_times = 0;
    }else {
        faild_times++;
    }
}