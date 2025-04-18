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
    void drawExplosionAnimation();
    bool isFired();
    Position2D getPosition();
    Position2D getCenterPosition();
    int getSize();

    void setSpeedMultiplier(float multiplier);  // <--- 新增：设置速度倍率

private:
    N5110 &_lcd;
    Joystick &_joystick;
    InterruptIn &_buttonB;
    Position2D _position;
    int _size;
    volatile bool _fireTriggered;
    Timer _debounceTimer;

    float _speedMultiplier;  // <--- 新增：速度倍率

    void constrainPosition();
    void _fireHandler();
};

#endif
