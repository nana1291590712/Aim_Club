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
    GameEngine(N5110 &lcd, Joystick &joystick, DigitalIn &buttonA, InterruptIn &buttonB);
    void init();
    void run();

private:
    N5110 &_lcd;
    Joystick &_joystick;
    DigitalIn &_buttonA;
    InterruptIn &_buttonB;

    Aim _aim;
    Target _target;

    Timer _gameTimer;
    bool _gameOver;
    int _mode;          // 0 = timed, 1 = endless
    float _timeLimit;   // 秒数限制

    void update();
    void draw();
    void checkGameState();
};

#endif
