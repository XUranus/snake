//
// Created by xuranus on 9/19/18.
//

#include "../header/zone.h"
#include <curses.h>
#include <string.h>
#include <iostream>

using namespace std;

zone::zone(int _row, int _column)
{
    row = _row;
    column = _column;

    //init color pair
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    clear();
    const char* msg = "Waiting Other Players....";
    mvaddnstr(1,1,msg,strlen(msg));
}


void zone::clear()
{
    ::clear();
    //print_border();
}

void zone::mvprintch(int x,int y,char ch, int color_pair_n) { // x >=0 && y>=0 (except border) default lch = rch = ch
    attron(COLOR_PAIR(color_pair_n));
    mvaddch(x,2*y,ch);
    mvaddch(x,2*y+1,ch);
    attroff(COLOR_PAIR(color_pair_n));
    ::refresh();
}

void zone::mvprintch(int x, int y,char chl,char chr,int color_pair_n) { // has left char and right char
    attron(COLOR_PAIR(color_pair_n));
    mvaddch(x,2*y,chl);
    mvaddch(x,2*y+1,chr);
    attroff(COLOR_PAIR(color_pair_n));
    ::refresh();
}



void zone::print_border(char ch, int color_pair_n) {
    for(int i=0;i<column;i++)
        mvprintch(0,i,ch,color_pair_n);
    for(int i=0;i<column;i++)
        mvprintch(row-1,i,ch,color_pair_n);
    for(int i=0;i<row;i++)
        mvprintch(i,0,ch,color_pair_n);
    for(int i=0;i<row;i++)
        mvprintch(i,column-1,ch,color_pair_n);
}