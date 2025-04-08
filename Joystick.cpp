#include "Joystick.h"
#include "math.h"

Joystick::Joystick(PinName vertPin, PinName horizPin)
    : _sensitivity(SENSITIVITY), _max_speed(MAX_SPEED) {
    vert = new AnalogIn(vertPin);
    horiz = new AnalogIn(horizPin);
    _current_speed = {0.0f, 0.0f};
}

void Joystick::init() {
    // 校准中心点
    _x0 = horiz->read();
    _y0 = vert->read();
}

Vector2D Joystick::_get_raw_coord() {
    // 原始读取并转换到 -1.0 ~ 1.0 范围
    float x = 2.0f * (horiz->read() - _x0);
    float y = 2.0f * (vert->read() - _y0);

    // 确保 y 方向正确
    return {x, y};  // 修正：不反转 Y 方向
}

Vector2D Joystick::_smooth_movement(Vector2D input) {
    float target_x = input.x * _max_speed * _sensitivity;
    float target_y = input.y * _max_speed * _sensitivity;

    _current_speed.x += (target_x - _current_speed.x) * ACCELERATION;
    _current_speed.y += (target_y - _current_speed.y) * ACCELERATION;

    float current_mag = sqrt(_current_speed.x * _current_speed.x + _current_speed.y * _current_speed.y);
    if (current_mag > _max_speed) {
        float scale = _max_speed / current_mag;
        _current_speed.x *= scale;
        _current_speed.y *= scale;
    }

    return _current_speed;
}

Vector2D Joystick::get_mapped_coord() {
    Vector2D raw = _get_raw_coord();
    return _smooth_movement(raw);  // 已取消死区处理
}

Direction Joystick::get_direction() {
    Vector2D coord = get_mapped_coord();
    float angle = atan2(coord.y, coord.x) * 57.2958f;

    if (is_centered()) {
        return CENTRE;
    }

    if (angle < 0) angle += 360.0f;

    if (angle < 22.5f || angle >= 337.5f) return E;
    if (angle < 67.5f) return NE;
    if (angle < 112.5f) return N;
    if (angle < 157.5f) return NW;
    if (angle < 202.5f) return W;
    if (angle < 247.5f) return SW;
    if (angle < 292.5f) return S;
    return SE;
}

float Joystick::get_magnitude() {
    Vector2D coord = get_mapped_coord();
    return sqrt(coord.x * coord.x + coord.y * coord.y);
}

bool Joystick::is_centered() {
    Vector2D coord = get_mapped_coord();
    return (fabs(coord.x) < 0.01f && fabs(coord.y) < 0.01f);
}

void Joystick::set_sensitivity(float sensitivity) {
    _sensitivity = sensitivity;
}

void Joystick::set_max_speed(float max_speed) {
    _max_speed = max_speed;
}
