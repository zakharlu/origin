#include <iostream>
#include "Counter.h"

Counter::Counter(int cnt_val) {
    this->cnt_val = cnt_val;
}

void Counter::increment() {
    cnt_val++;
}

void Counter::decrement() {
    cnt_val--;
}

void Counter::print() {
    std::cout << cnt_val << std::endl;
}