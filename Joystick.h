#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "mbed.h"
#include "Utils.h"

// 可配置参数
#define MAX_SPEED 3.3f        // 最大移动速度（原为 5.0f，降低了 1/3）
#define ACCELERATION 0.2f     // 加速度系数
#define SENSITIVITY 1.0f      // 灵敏度系数

class Joystick {
public:
    Joystick(PinName vertPin, PinName horizPin);
    
    void init();                    // 初始化摇杆
    Vector2D get_mapped_coord();    // 获取平滑处理后的坐标
    Direction get_direction();      // 获取方向
    float get_magnitude();          // 获取摇杆偏移幅度
    bool is_centered();             // 检查摇杆是否在中心位置
    
    void set_sensitivity(float sensitivity);
    void set_max_speed(float max_speed);

private:
    AnalogIn *vert;
    AnalogIn *horiz;

    float _x0;
    float _y0;

    float _sensitivity;
    float _max_speed;

    Vector2D _current_speed;

    Vector2D _get_raw_coord();
    Vector2D _smooth_movement(Vector2D input);
};

#endif
