//
// Created by xuranus on 9/21/18.
//

#include "../header/server.h"
#include "../header/gameMessage.pb.h"
#include<iostream>
#include<cstdio>
#include<cstring>
#include<ctime>
#include <algorithm>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/time.h>
#include<sys/select.h>

#include<signal.h>
#include<vector>

#include "../header/timer.h"
#include "../header/zone.h"

server::server(int port ,int _client_num,int zone_row,int zone_col) {
    //创建服务器端socket，地址族为AF_INET(IPv4)，传输方式为TCP
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    //初始化监听IP为本地所有IP，端口为已设置的port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    client_num = _client_num;
    client_socket.clear();
    timeout = {0,1000*10};

    active = true;//actived
    slot = 0;// t is the mininum slot
    frame = 0;//frame is the number of frame

    printf("new server created,port = %d\n",port);
    battle_zone = new zone(zone_row,zone_col,_client_num);
    printf("battle_field generated.\n");
}

void server::stop() { // stop the server
    active = false;
    close(server_socket);
    printf("server stopped.\n");
}

bool server::start() { //start the server
    printf("starting server...\n");
    //bind socket and IP
    int ret = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0) //bind failed
    {
        close(server_socket);
        server_socket = -1;
        perror("server start failed. ");
        return false;
    }

    //listen status
    listen(server_socket, 10);

    //config client socket
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    client_addr_len = sizeof(client_addr);
    //waiting for join
    printf("waiting for %d players to connect...\n",client_num);
    while(client_socket.size()<client_num && active) {
        int new_fd = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if(new_fd!=-1) {
            client_socket.push_back(new_fd);
            printf("%d players has joined room\n", (int)client_socket.size());
        }
        else {
            std::cout << "one player join room failed." << std::endl;
        }
    }
    printf("game ready.\n\n");

    timer _timer(1000*100);// 1000*1000 ns slot update sync


    printf("the are %ld snakes in total\n",battle_zone->snakes.size());
    for(int i=0;i<client_num;i++) battle_zone->snakes[i]->debug();
    printf("------------slot and frame start to count----------------\n\n");



    while(active) {
        if(!_timer.count_down()) fetch_data();
        else send_sync_data();

        printf("frame.slot[%lld,%lld]:\n",frame,++slot);
        //for(int i=0;i<client_num;i++) battle_zone->snakes[i]->debug();
    }

    return true;
}

void server::fetch_data() { //every time fetch data cost a frame
    FD_ZERO(&client_fdset);
    timeout = {0,1000*10};
    int max_client_fd = -1;
    for(int i=0;i<client_num;i++) {
        if(max_client_fd<client_socket[i])
            max_client_fd = client_socket[i];
        FD_SET(client_socket[i],&client_fdset);
    }


    int ret = select(max_client_fd+1,&client_fdset, nullptr, nullptr,&timeout);
    if(ret < 0) std::cout << "select error." << std::endl;
    //else if(ret == 0) std::cout << "select timeout." << std::endl;
    else {
        for(int i=0;i<client_num;i++) {
            if(FD_ISSET(client_socket[i],&client_fdset)) {
                char buf[50];
                bzero(buf,50);
                read(client_socket[i],buf,50);
                //printf("client:%d %s\n",i,buf);
                battle_zone->deal_move_message(i,buf);
                continue;
            }
        }
    }

}

void server::send_sync_data() {
    //process
    printf("\nframe[%lld]\n",++frame);
    battle_zone->process();
    const GameProto::ServerMessage &msg = battle_zone->server_msg_proto_data();


    //cout << "send bytes = " << msg.ByteSize() << endl;
    cout << "send msg : " << msg.DebugString() << endl;

    void* buf = malloc((size_t)msg.ByteSize());
    if(buf== nullptr) cout << "malloc failed" << endl;
    bool ret = msg.SerializeToArray(buf,msg.ByteSize());

    //cout << "Seriliaze bufsize = " << sizeof(buf) << endl;
    //std::cout << "serialize ret  = " << ret << std::endl;


    //broadcast
    std::cout << "sending sync data.." << std::endl;
    for(int i=0;i<client_num;i++) {
        send(client_socket[i], buf, msg.ByteSize(),0);
        //printf("send buf size = %d\n", msg.ByteSize());
    }

    printf("-------------------------------------------------\n");
}

server::~server() {
    stop();
}