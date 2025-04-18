#include "SpecialTarget.h"

SpecialTarget::SpecialTarget(N5110 &lcd) : _lcd(lcd), _active(false), _duration(5000) {}

void SpecialTarget::init() {
    _active = false;
}

void SpecialTarget::generate() {
    _pos.x = rand() % (84 - 10) + 5;
    _pos.y = rand() % (48 - 10) + 5;
    _active = true;
    _effect = static_cast<SpecialEffect>(rand() % 3);
    _timer.reset();
    _timer.start();
}

bool SpecialTarget::isActive() {
    if (_active && _timer.elapsed_time() >= 5000ms) {
        _active = false;
    }
    return _active;
}

void SpecialTarget::update() {
    if (isActive()) {
        // nothing dynamic yet
    }
}

void SpecialTarget::draw() {
    if (!_active) return;

    // 绘制7x7的正方形边框
    _lcd.drawRect(_pos.x - 3, _pos.y - 3, 7, 7, FILL_TRANSPARENT);
    
    // 绘制更清晰美观的问号
    drawQuestionMark(_pos);
}

bool SpecialTarget::checkHit(Position2D aimPos) {
    if (!_active) return false;

    int dx = aimPos.x - _pos.x;
    int dy = aimPos.y - _pos.y;
    if (dx * dx + dy * dy <= 16) {
        _active = false;
        return true;
    }
    return false;
}

SpecialEffect SpecialTarget::getEffect() {
    return _effect;
}

void SpecialTarget::drawQuestionMark(Position2D center) {
    // 清除原有点阵绘制方式，改用更清晰的问号
    
    // 问号顶部圆弧
    _lcd.setPixel(center.x, center.y - 2);
    _lcd.setPixel(center.x - 1, center.y - 1);
    _lcd.setPixel(center.x + 1, center.y - 1);
    
    // 问号中间点
    _lcd.setPixel(center.x, center.y);
    
    // 问号底部点
    _lcd.setPixel(center.x, center.y + 2);
    
    // 可选：添加更多像素点使问号更明显
    _lcd.setPixel(center.x - 1, center.y - 2);
    _lcd.setPixel(center.x + 1, center.y - 2);
}
