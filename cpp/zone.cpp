//
// Created by xuranus on 9/19/18.
//

#include "../header/zone.h"
#include "../header/food.h"
#include "../header/body_node.h"
#include "../header/gameMessage.pb.h"

#include <string.h>
#include <iostream>
#include <cstdlib>
#include <list>
#include <cstdlib>


using namespace std;


zone::zone(int _row, int _column, int _snake_num)
{
    row = _row;
    column = _column;
    snake_num = _snake_num;

    //allowcate array map
    map = new int*[row];
    for(int i=0;i<row;i++) {
        auto t = new int[column];
        map[i] = t;
    }
    for(int i=0;i<row;i++)
        for(int j=0;j<column;j++)
            map[i][j] = 0;

    for(int i=0;i<column;i++) map[0][i] = P_WALL;
    for(int i=0;i<column;i++) map[row-1][i] = P_WALL;
    for(int i=0;i<row;i++) map[i][0] = P_WALL;
    for(int i=0;i<row;i++) map[i][column-1] = P_WALL;

    snakes.clear();
    for(int i=0;i<snake_num;i++) {
        snakes.push_back(init_snake());
    }
    foods.clear();
}


zone::~zone() {
    for(int i=0;i<row;i++)
        delete map[i];
}

snake* zone::init_snake() {
    snake* s = snake::init_random_snake(this);
    set_snake(s->head->x,s->head->y);
    return s;
}


void zone::set_food(int x, int y) {
    printf("call set_food(%d,%d)\n",x,y);
    food f(x,y);
    foods.push_back(f);
    map[x][y] = P_FOOD;
}

void zone::set_nothing(int x, int y) {
    printf("call set_nothing(%d,%d)\n",x,y);
    map[x][y] = P_NOTHING;
}

void zone::set_snake(int x, int y) {
    printf("call set_snake(%d,%d)\n",x,y);
    map[x][y] = P_SNAKE;
}

void zone::add_random_food() {
    printf("call add_random_food\n");
    long long x = 0,y = 0;
    int times = 0;
    while(!has_nothing(x,y) && times++<100) { // prevent dead lock
        x = random()%row;
        y = random()%column;
    }
    if(x*y!=0) set_food((int)x,(int)y);
}

bool zone::has_food(int x, int y) {
    bool res = (map[x][y]==P_FOOD);
    printf("call has_food(%d,%d) return %d\n",x,y,res);
    return res;
}

void zone::delete_food(int x, int y) {
    printf("call delete_food(%d,%d)\n",x,y);
    for(list<food>::const_iterator iter = foods.begin();iter != foods.end(); iter++) {
        if(iter->x == x && iter->y == y) {
            foods.erase(iter);
            map[x][y] = P_NOTHING;
            return;
        }
    }
}


bool zone::has_snake(int x, int y) {
    bool res = (map[x][y] == P_SNAKE);
    printf("call has_snake(%d,%d) return %d\n",x,y,res);
    return res;
}

bool zone::has_wall(int x, int y) {
    bool res = (map[x][y] == P_WALL);
    printf("call has_wall(%d,%d) return %d\n",x,y,res);
    return res;
}

bool zone::has_nothing(int x, int y) {
    bool res = (map[x][y] == P_NOTHING);
    printf("call has_nothing(%d,%d) return %d\n",x,y,res);
    return res;
}


void zone::process() {
    printf("processing data...\n");
    vector<int> dead_list;
    for(int i=0;i<snakes.size();i++) {
        bool res = snakes[i]->move();
        if(!res) { // snake i loss
            printf("snake %d die\n",i);
            dead_list.push_back(i);
        }
    }
    for(int i=0;i<dead_list.size();i++) {
        snakes[dead_list[i]]->reborn();
        printf("snake %d reborned\n",dead_list[i]);
    }
}


body_node* zone::spare_random_node() {
    long long x = 0,y = 0;
    while(!has_nothing(x,y) || x*y==0) { // prevent dead lock
        x = random()%row;
        y = random()%column;
    }
    return new body_node((int)x,(int)y, nullptr, nullptr);
}

void zone::deal_move_message(int i, char *msg) {
    printf("zone get client %d message: %s\n",i,msg);
    string dir(msg);
    snakes[i]->set_direct(dir);
}

void zone::print_map() {//used to debug
    printf("\n");
    for(int i=0;i<row;i++) {
        for(int j=0;j<column;j++) {
            /*
             * P_NOTHING=0,
                P_WALL=1,
                P_SNAKE=2,
                P_FOOD=3
             * */
            if(map[i][j]==0) putchar('_');
            else if(map[i][j]==1) putchar('W');
            else if(map[i][j]==2) putchar('S');
            else if(map[i][j]==3) putchar('F');
            else putchar('?');
        }
        printf("\n");
    }
}

GameProto::ServerMessage zone::server_msg_proto_data() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    GameProto::ServerMessage msg;
    msg.set_action(GameProto::ServerMessage_ActionType_SyncMap);
    string message = "Test Message";
    msg.set_msg(message);
    GameProto::BattleField* battleField = msg.mutable_battle_field();

    for(int i=0;i<snakes.size();i++) {
        GameProto::Snake* ts = battleField->add_snake();
        ts->set_color(i);
        for(auto node = snakes[i]->head; node!= nullptr;node = node->next) {
            GameProto::Point* p= ts->add_node();
            p->set_x(node->x);
            p->set_y(node->y);
        }
    }

    for(auto food:foods) {
        GameProto::Food* tf = battleField->add_food();
        tf -> set_x(food.x);
        tf -> set_y(food.y);
    }

    return msg;
}