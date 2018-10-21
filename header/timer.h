//
// Created by xuranus on 10/11/18.
//

#ifndef SNAKE_TIMER_H
#define SNAKE_TIMER_H

#include <signal.h>
#include <time.h>
#include <chrono>

struct timer {
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::microseconds res;

    int duration;
    clock::time_point start,last;

    timer(int);
    bool count_down();
};


#endif //SNAKE_TIMER_H
