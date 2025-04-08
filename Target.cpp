#include "Target.h"
#include "math.h"

#define PI 3.14159265358979323846f

Target::Target(N5110 &lcd) : _lcd(lcd) {
    _size = 3;  // Base size (radius for circle)
}

void Target::init() {
    srand(time(NULL));  // Seed random number generator
    _size = 3;
}

void Target::generate() {
    randomizePosition();
    _type = rand() % 3;  // Randomly select square/triangle/circle
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
        case 0:  // Hollow 7x7 square → only hit if center is on the border
            return (abs(dx) == 3 && abs(dy) <= 3) || (abs(dy) == 3 && abs(dx) <= 3);

        case 1:  // Triangle approximation
            return (dy >= -4 && dy <= 0) && (abs(dx) <= ((3 * (4 + dy)) / 4));

        case 2:  // Circle, radius 3
            return distanceSquared <= 9;

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
    // Draw a 7x7 hollow square
    int size = 7;
    _lcd.drawRect(_position.x, _position.y, size, size, FILL_TRANSPARENT);
}

void Target::drawTriangle() {
    // Triangle with base 7 and height 4
    int baseHalf = 4;  // 8 / 2
    int height = 5;

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
    _lcd.drawCircle(_position.x, _position.y, 3, FILL_TRANSPARENT);
}

void Target::randomizePosition() {
    int margin = 4 + 2;  // 4 for half of max shape, 2 for extra padding
    _position.x = margin + rand() % (84 - 2 * margin);
    _position.y = margin + rand() % (48 - 2 * margin);
}
