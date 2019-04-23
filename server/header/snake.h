//
// Created by xuranus on 9/19/18.

#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include "body_node.h"
#include <string>
struct zone;

struct snake {/*user keyboard control status*/
    enum DIRECT {
        UP = 1,
        DOWN = -1,
        LEFT = 2,
        RIGHT = -2,
        UNKNOWN = 0
    };

    body_node *head,*tail; //snake head and tail
    DIRECT direct;
    int length;
    zone* _zone;

    static snake* init_random_snake(zone*);
    snake(int , int, zone*);
    snake(zone*);
    ~snake();
    bool move();
    void eat(int, int);
    bool loss();
    void killed();
    void reborn();

    void set_direct(std::string);

    void debug();
};


#endif