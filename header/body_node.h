//
// Created by xuranus on 9/19/18.
//

#ifndef SNAKE_BODY_NODE_H
#define SNAKE_BODY_NODE_H


struct body_node {
    int x,y; //coordinate of the snake snake node
    body_node *pre,*next; //previous node and next node

    body_node(int , int);
    body_node(int ,int ,body_node*,body_node*);
};


#endif //SNAKE_BODY_NODE_H
