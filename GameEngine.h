#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "mbed.h"
#include "N5110.h"
#include "Joystick.h"
#include "Aim.h"
#include "Target.h"
#include "Utils.h"

class GameEngine {
public:
    // In GameEngine.h
GameEngine(N5110 &lcd, Joystick &joystick, DigitalIn &buttonA, InterruptIn &buttonB);
    void init();
    void run();

private:
    // In GameEngine.h private section
    DigitalIn &_buttonA;
    N5110 &_lcd;
    Joystick &_joystick;
    InterruptIn &_buttonB;
    Aim _aim;
    Target _target;
    bool _gameOver;

    void update();
    void draw();
    void checkGameState();
};

#endif