

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
    int maxX;  // Declare maxX outside the switch for triangle case

    switch (_type) {
        case 0: // Square (8x8)
            // Check if within the square's bounds
            return (abs(dx) <= 5 && abs(dy) <= 5);
            
        case 1: // Triangle (base 12px, height 7px)
            // Triangle specific check:
            // The apex is at (_position.x, _position.y - 7)
            // The base goes from (_position.x-6, _position.y) to (_position.x+6, _position.y)
            if (dy > 0 || dy < -7) return false; // Outside vertical bounds
            // Calculate the horizontal bounds based on y position
            maxX = (6 * (7 + dy)) / 7; // Linearly decreases from 6 at top to 0 at bottom
            return abs(dx) <= maxX;
            
        case 2: // Circle (radius 4)
            return distanceSquared <= 16; // 4^2 = 16
            
        default:
            return false;
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

        // 根据不同类型设置不同的边界检测
        switch (_type) {
            case 0: // 正方形 (8x8)
                if (_position.x <= 4 || _position.x >= 84 - 8) _direction.x *= -1;
                if (_position.y <= 4 || _position.y >= 48 - 8) _direction.y *= -1;
                break;
                
            case 1: // 三角形 (底边12像素，高7像素)
                if (_position.x <= 6 || _position.x >= 84 - 6) _direction.x *= -1;
                if (_position.y <= 7 || _position.y >= 48) _direction.y *= -1;
                break;
                
            case 2: // 圆形 (半径4)
                if (_position.x <= 4 || _position.x >= 84 - 4) _direction.x *= -1;
                if (_position.y <= 4 || _position.y >= 48 - 4) _direction.y *= -1;
                break;
        }

        _lastMoveTime = now;
    }
}

int Target::getType() {
    return _type;
}
