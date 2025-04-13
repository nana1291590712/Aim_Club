#ifndef TARGET_H
#define TARGET_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"

class Target {
public:
    Target(N5110 &lcd);
    void init();
    void generate();
    void draw();
    bool checkHit(Position2D pos);
    Position2D getPosition();
    int getSize();
    void move();  // 新增：缓慢移动

private:
    N5110 &_lcd;
    Position2D _position;
    int _type;
    int _size;

    Timer _timer;
    int _moveInterval;
    std::chrono::milliseconds _lastMoveTime;
    Position2D _direction;

    void drawSquare();
    void drawTriangle();
    void drawCircle();
    void randomizePosition();
    void randomizeDirection();
};

#endif
