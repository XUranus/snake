#include <iostream>
#include <curses.h>
#include <signal.h>
#include <getopt.h>
#include "header/server.h"
#include "header/zone.h"

using namespace std;

struct server * s;

void stop_server(int sign_num) {
    cout << "signal detected, stoping server..." << endl;
    s->stop();
}

void print_help() {
    cout << "usage: snake_server -p <port> -n <players> -c <colns> -r <rowns>" << endl;
}

int main(int argc, char *argv[]) {
    signal(SIGINT,stop_server);

    int port = 0, players_num = 0, row_num = 30, col_num = 50;
    
    int opt;
    const char *str = "hp:n:c:r:";
    while ((opt = getopt(argc, argv, str)) != -1) {
        switch (opt) {
            case 'p': {
                port = atoi(optarg);
                break;
            }
            case 'n': {
                players_num = atoi(optarg);
                break;
            }
            case 'c': {
                col_num = atoi(optarg);
                break;
            }
            case 'r': {
                row_num = atoi(optarg);
                break;
            }
            case 'h': {
                print_help();
                exit(0);
                break;
            }
            default:
                break;
        }
    }

    if(port <= 0 || players_num <= 0 || row_num <= 0 || col_num <= 0) {
        cout << "illegal args." << endl;
        exit(1);
    }

    cout << "port: " << port << endl;
    cout << "players_num: " << players_num << endl;
    cout << "zone_row: " << row_num << endl;
    cout << "zone_col:" << col_num << endl;

    //port(room_id) ,client_num, zone_row, wzone_col
    s = new server(port, players_num, row_num, col_num);

    if(!s->start())
    {
        perror("server");
        delete s;
    }


    return 0;
}
