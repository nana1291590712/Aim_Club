#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "mbed.h"
#include "N5110.h"
#include "Joystick.h"
#include "Aim.h"
#include "Target.h"
#include "Utils.h"
#include "SpecialTarget.h"

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
    SpecialTarget _specialTarget;

    DigitalOut *_redLEDs;
    DigitalOut &_greenLED;

    Timer _gameTimer;
    Timer _effectTimer;

    bool _gameOver;
    int _mode;                // 0 = timed, 1 = endless
    float _timeLimit;
    int _lives;

    bool _freezeTargets;
    bool _slowAim;
    bool _effectMessageShown;
    float _nextSpecialTime;

    int _circleHits;
    int _squareHits;
    int _triangleHits;

    void update();
    void draw();
    void checkGameState();
    void updateLivesDisplay();
    void loseLife();
    void flashGreenLED();

    void applyEffect(SpecialEffect effect);
    void drawEffectMessage();
};

#endif
