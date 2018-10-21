//
// Created by xuranus on 9/19/18.
//

#include "../header/body_node.h"

body_node::body_node(int _x, int _y, body_node *_pre, body_node *_next) {
    x = _x;
    y = _y;
    pre = _pre;
    next = _next;
}

body_node::body_node(int _x, int _y) {
    x = _x;
    y = _y;
    next = pre = nullptr;
}