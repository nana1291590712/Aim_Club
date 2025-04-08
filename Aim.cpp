#include "Aim.h"

Aim::Aim(N5110 &lcd, Joystick &joystick, InterruptIn &buttonB) 
    : _lcd(lcd), _joystick(joystick), _buttonB(buttonB) {
    _size = 5;
    _fireTriggered = false;
}

void Aim::init() {
    _position.x = 84 / 2;
    _position.y = 48 / 2;
    _buttonB.fall(callback(this, &Aim::_fireHandler));  // 绑定中断回调
    _debounceTimer.start();
}

void Aim::_fireHandler() {
    if (_debounceTimer.elapsed_time() > 50ms) {  // 防抖处理
        _fireTriggered = true;
        _debounceTimer.reset();
    }
}

bool Aim::isFired() {
    if (_fireTriggered) {
        _fireTriggered = false;
        return true;
    }
    return false;
}

void Aim::update() {
    Vector2D coord = _joystick.get_mapped_coord();
    float sensitivity = 2.0f;
    _position.x += (int)(coord.x * sensitivity);
    _position.y += (int)(coord.y * sensitivity);
    constrainPosition();
}

void Aim::draw() {
    int halfSize = _size / 2;
    _lcd.drawLine(_position.x - halfSize, _position.y, 
                 _position.x + halfSize, _position.y, 1);
    _lcd.drawLine(_position.x, _position.y - halfSize, 
                 _position.x, _position.y + halfSize, 1);
}

Position2D Aim::getPosition() {
    return _position;
}

Position2D Aim::getCenterPosition() {
    return _position;
}

int Aim::getSize() {
    return _size;
}

void Aim::constrainPosition() {
    if (_position.x < _size)
        _position.x = _size;
    else if (_position.x > 84 - _size)
        _position.x = 84 - _size;
    
    if (_position.y < _size)
        _position.y = _size;
    else if (_position.y > 48 - _size)
        _position.y = 48 - _size;
}
