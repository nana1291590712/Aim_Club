#include "Target.h"
#include <cmath>

Target::Target(N5110 &lcd) : _lcd(lcd) {
    _size = 3;
    _moveInterval = 200;
    _direction = {1, 1};
}

void Target::init() {
    srand(time(NULL));
    _size = 3;
    _timer.start();
    _lastMoveTime = 0ms;
    generate();
}

void Target::generate() {
    randomizePosition();
    _type = rand() % 3;
    randomizeDirection();
    _lastMoveTime = std::chrono::duration_cast<std::chrono::milliseconds>(_timer.elapsed_time());
}

void Target::draw() {
    switch (_type) {
        case 0: drawSquare(); break;
        case 1: drawTriangle(); break;
        case 2: drawCircle(); break;
    }
}

bool Target::checkHit(Position2D pos) {
    int dx = pos.x - _position.x;
    int dy = pos.y - _position.y;
    int distanceSquared = dx * dx + dy * dy;

    switch (_type) {
        case 0: return (abs(dx) == 3 && abs(dy) <= 3) || (abs(dy) == 3 && abs(dx) <= 3);
        case 1: return (dy >= -4 && dy <= 0) && (abs(dx) <= ((3 * (4 + dy)) / 4));
        case 2: return distanceSquared <= 9;
        default: return false;
    }
}

Position2D Target::getPosition() {
    return _position;
}

int Target::getSize() {
    return _size;
}

void Target::drawSquare() {
    _lcd.drawRect(_position.x, _position.y, 8, 8, FILL_TRANSPARENT);
}

void Target::drawTriangle() {
    int baseHalf = 6;
    int height = 7;
    int x1 = _position.x;
    int y1 = _position.y - height;
    int x2 = _position.x - baseHalf;
    int y2 = _position.y;
    int x3 = _position.x + baseHalf;
    int y3 = _position.y;
    _lcd.drawLine(x1, y1, x2, y2, 1);
    _lcd.drawLine(x2, y2, x3, y3, 1);
    _lcd.drawLine(x3, y3, x1, y1, 1);
}

void Target::drawCircle() {
    _lcd.drawCircle(_position.x, _position.y, 4, FILL_TRANSPARENT);
}

void Target::randomizePosition() {
    int margin = 6;
    _position.x = margin + rand() % (84 - 2 * margin);
    _position.y = margin + rand() % (48 - 2 * margin);
}

void Target::randomizeDirection() {
    _direction.x = (rand() % 3) - 1;
    _direction.y = (rand() % 3) - 1;
    if (_direction.x == 0 && _direction.y == 0) {
        _direction.x = 1;
    }
}

void Target::move() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(_timer.elapsed_time());
    if ((now - _lastMoveTime).count() >= _moveInterval) {
        _position.x += _direction.x;
        _position.y += _direction.y;

        if (_position.x < 4 || _position.x > 80) _direction.x *= -1;
        if (_position.y < 4 || _position.y > 44) _direction.y *= -1;

        _lastMoveTime = now;
    }
}
