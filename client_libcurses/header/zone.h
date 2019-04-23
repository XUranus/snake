//
// Created by xuranus on 9/19/18.
//

#ifndef SNAKE_CLIENT_ZONE_H
#define SNAKE_CLIENT_ZONE_H


struct zone {
    /*
     * battle zone
     *
     * */
    int row,column; //this is not equal to curses row and column

    zone(int , int);

    void clear();

    void mvprintch(int ,int ,char,int);
    void mvprintch(int ,int ,char,char,int);

    void print_border(char, int);
};


#endif //SNAKE_CLIENT_ZONE_H
