#include "mbed.h"
#include "Joystick.h"
#include "N5110.h"
#include "Utils.h"
#include "GameEngine.h"
#include "Aim.h"
#include "Target.h"

// ------------ 硬件引脚定义 -----------------
N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);
Joystick joystick(PC_1, PC_0);
DigitalIn buttonA(BUTTON1); // Start button
InterruptIn buttonB(PC_5);  // Shooting button
InterruptIn buttonC(PC_10); // Timed mode
InterruptIn buttonD(PC_12); // Endless mode
DigitalOut redLEDs[3] = {PB_15, PB_14, PB_13};  // Remaining Lives Indicator
DigitalOut greenLED(PC_6); // Shooting feedback

// ------------ 函数原型 -----------------
void init();
void welcome();
void game_over();

int main() {
    init();      // 初始化设备
    welcome();   // 等待玩家按下开始按钮
    
    // 创建并启动游戏引擎
    GameEngine game(lcd, joystick, buttonA, buttonB);
    game.init();
    game.run();

    game_over(); // 游戏结束界面
}

void init() {
    lcd.init(LPH7366_1);
    lcd.setContrast(0.5);
    joystick.init(); 
}

void welcome() {
    lcd.printString("Game Start!", 0, 1);  
    lcd.printString("Press Nucleo", 0, 3);
    lcd.printString("Blue button", 0, 4);
    lcd.refresh();

    while (buttonA.read() == 1) {
        ThisThread::sleep_for(100ms);
    }
}

void game_over() {
    while (true) {
        lcd.clear();
        lcd.printString("  Game Over ", 0, 2);  
        lcd.refresh();
        ThisThread::sleep_for(250ms);
        lcd.clear();
        lcd.refresh();
        ThisThread::sleep_for(250ms);
    }
}
