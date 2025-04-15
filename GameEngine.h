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
    GameEngine(N5110 &lcd, Joystick &joystick, DigitalIn &buttonA, InterruptIn &buttonB, int mode, DigitalOut redLEDs[], DigitalOut &greenLED);
    void init();
    void run();
    void handleHit(Position2D aimPos);
    void getStats(int &score, int &circleCount, int &squareCount, int &triangleCount);

private:
    N5110 &_lcd;
    Joystick &_joystick;
    DigitalIn &_buttonA;
    InterruptIn &_buttonB;

    Aim _aim;
    Target _target;
    DigitalOut *_redLEDs;      // Array of red LEDs for lives
    DigitalOut &_greenLED;     // Green LED for hit feedback

    Timer _gameTimer;
    bool _gameOver;
    int _mode;                 // 0 = timed, 1 = endless
    float _timeLimit;          // Time limit in seconds
    int _lives;                // Player lives count

    void update();
    void draw();
    void checkGameState();
    void updateLivesDisplay();
    void loseLife();
    void flashGreenLED();      // Flash green LED for feedback

    int _circleHits = 0;
    int _squareHits = 0;
    int _triangleHits = 0;

};

#endif
