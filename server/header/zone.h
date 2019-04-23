//
// Created by xuranus on 9/19/18.
//

#ifndef SNAKE_ZONE_H
#define SNAKE_ZONE_H

#include <vector>
#include <list>
#include <iostream>

#include "food.h"
#include "body_node.h"
#include "snake.h"
#include "gameMessage.pb.h"

using namespace std;


struct zone {
    /*
     * battle zone
     *
     * */
    int row,column; //this is not equal to curses row and column
    int** map;

    enum p_MAP {
        P_NOTHING=0,
        P_WALL=1,
        P_SNAKE=2,
        P_FOOD=3
    };

    int snake_num;
    vector<snake*> snakes;
    list<food> foods;


    zone(int , int , int );
    ~zone();

    snake* init_snake();
    body_node* spare_random_node();

    void add_random_food();
    void set_food(int, int);
    bool has_food(int,int);
    void delete_food(int, int);

    bool has_nothing(int,int);
    void set_nothing(int,int);

    bool has_snake(int,int);
    void set_snake(int,int);;

    bool has_wall(int,int);

    void print_map();//debug
    GameProto::ServerMessage server_msg_proto_data();//generate proto data


    void deal_move_message(int,char*);//recevice clients movement message
    void process();//just do process



};


#endif //SNAKE_CLIENT_ZONE_H
