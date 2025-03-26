#pragma once
class Counter {
    int cnt_val;
public:
    Counter(int cnt_val);
    void increment();
    void decrement();
    void print();
};