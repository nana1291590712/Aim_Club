
#ifndef SPECIAL_TARGET_H
#define SPECIAL_TARGET_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"

enum class SpecialEffect {
    AddTime,         // 限时模式专用
    FreezeTargets,
    SlowAim,
    AddLife          // 无尽模式专用
};


class SpecialTarget {
public:
    SpecialTarget(N5110 &lcd);
    void init();
    void update();
    void draw();
    bool isActive();
    void generate(int mode);
    bool checkHit(Position2D aimPos);
    SpecialEffect getEffect();

private:
    N5110 &_lcd;
    bool _active;
    Position2D _pos;
    Timer _timer;
    int _duration; // ms
    SpecialEffect _effect;

    void drawQuestionMark(Position2D center);
};

#endif
