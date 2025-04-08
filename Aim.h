#ifndef AIM_H
#define AIM_H

#include "mbed.h"
#include "N5110.h"
#include "Joystick.h"
#include "Utils.h"

class Aim {
public:
    Aim(N5110 &lcd, Joystick &joystick, InterruptIn &buttonB);
    void init();
    void update();
    void draw();
    bool isFired();
    Position2D getPosition();
    Position2D getCenterPosition();
    int getSize();

private:
    N5110 &_lcd;
    Joystick &_joystick;
    InterruptIn &_buttonB;
    Position2D _position;
    int _size;
    volatile bool _fireTriggered;  // 中断标记射击状态
    Timer _debounceTimer;

    void constrainPosition();
    void _fireHandler();  // 中断回调函数
};

#endif