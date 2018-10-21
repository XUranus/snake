//
// Created by xuranus on 9/19/18.
//

#include "../header/snake.h"
#include "../header/zone.h"

snake* snake::init_random_snake(zone *_z) {
    //init a snake on zone
    snake* new_snake = new snake(_z);
    new_snake->head = new_snake->tail = _z->spare_random_node();
    new_snake->length = 1;
    printf("snake random generated:");
    new_snake->debug();
    return new_snake;
}

snake::snake(int x, int y,zone* _z)
{
    head = new body_node(x,y);
    tail = head;
    length = 1;

    direct = RIGHT;//default right
    _zone = _z;
}

snake::snake(zone *z) {
    head = tail = nullptr;
    length = 0;
    direct = RIGHT;
    _zone = z;
}

snake::~snake() {
    body_node* p = head;
    while(p!=nullptr) {
        body_node *t = p;
        p = p->next;
        delete t;
    }
}

bool snake::move() //just go head and process , if return false , you lose
{
    printf("moving snake...\n");//
    printf("before move:");debug();

    int next_x = -1,next_y = -1;
    if(direct == UP) {
        next_x = head->x - 1;
        next_y = head->y;
    } else if(direct == DOWN) {
        next_x = head->x + 1;
        next_y = head->y;
    } else if(direct == LEFT) {
        next_x = head->x;
        next_y = head->y-1;
    } else if(direct == RIGHT) {
        next_x = head->x;
        next_y = head->y+1;
    }

    printf("next_x=%d next_y=%d\n",next_x,next_y);//

    if(_zone->has_food(next_x,next_y)) {
        eat(next_x,next_y);
    } else if(_zone->has_nothing(next_x,next_y)) {
        //move on
        head->pre = new body_node(next_x,next_y, nullptr,head);
        head = head->pre;
        length++;
        _zone->set_snake(next_x,next_y);
        loss();
        //_zone->print_map();//
    } else if(_zone->has_snake(next_x,next_y)) {
        //loss_game;
        killed();
        //_zone->print_map();//
        return false;
    } else if(_zone->has_wall(next_x,next_y)) {
        //loss_game;
        killed();
        //_zone->print_map();//
        return false;
    }

    return true;
}


void snake::eat(int next_x, int next_y) {//eat a food and gain length
    printf("snake eat (%d,%d) and gain length\n",next_x,next_x);
    head->pre = new body_node(next_x,next_y, nullptr,head);
    head = head->pre;
    length++;

    //inform zone
    _zone->delete_food(next_x,next_y);
    _zone->set_snake(next_x,next_y);
}

bool snake::loss() //lose the tail node
{
    printf("snake loss one node\n");
    if(length<=1) return false;
    length--;
    tail->pre->next = nullptr;
    body_node *t = tail;
    tail = t->pre;

    //inform zone
    printf("inform zone to set nothing (%d,%d)\n",t->x,t->y);
    _zone->set_nothing(t->x,t->y);
    delete t;
    return true;
}

void snake::killed() {
    printf("snake killed\n");//
    body_node* p = head;
    while(p!= nullptr) {
        body_node *t = p;
        p = p->next;
        _zone->set_food(t->x,t->y);
        //_zone->set_nothing(t->x,t->y);
        delete t;
    }
    length = 0;
    head = tail = nullptr;
}

void snake::reborn() {
    printf("one snake reborn\n");
    head = tail = _zone->spare_random_node();
    direct = RIGHT;
    length = 1;
}

void snake::set_direct(std::string s) {
    DIRECT t_direct;
    if(s=="UP") t_direct = UP;
    else if(s=="DOWN") t_direct = DOWN;
    else if(s=="LEFT") t_direct = LEFT;
    else if(s=="RIGHT") t_direct = RIGHT;
    else t_direct = UNKNOWN;
    if(t_direct+direct!=0) direct = t_direct;//prevent sudden die
}

void snake::debug() {
    printf("[ ");
    body_node* p = head;
    while(p!= nullptr) {
        printf("(%d,%d) -> ",p->x,p->y);
        p = p->next;
    }
    printf("]\n");
}
