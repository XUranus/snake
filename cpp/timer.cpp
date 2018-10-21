//
// Created by xuranus on 10/11/18.
//

#include "../header/timer.h"
#include <iostream>

timer::timer(int _duration) // nsec
    : start(res::zero())
    , last(res::zero())
{
    duration = _duration;
}

bool timer::count_down() {
    clock::time_point now = clock::now();
    if(std::chrono::duration_cast<res>(now - last).count() < duration) {
        return false;
    } else { // count down
        last = clock::now();
        return true;
    }
}