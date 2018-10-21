#include <iostream>
#include <curses.h>
#include <signal.h>
#include "header/server.h"
#include "header/zone.h"

using namespace std;

struct server * s;


void stop_server(int sign_num) {
    cout << "signal detected, stoping server..." << endl;
    s->stop();
}

int main() {
    signal(SIGINT,stop_server);


    s = new server(8090,2,30,50);
    if(!s->start())
    {
        perror("server");
        delete s;
    }


    return 0;
}
