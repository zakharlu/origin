#pragma once
class Figure {
protected:
    int sides_cnt;
    std::string name;

public:
    Figure();
    virtual void get_sides_angles_value();
};