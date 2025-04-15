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
int welcome();  // 返回所选模式
void game_over(GameEngine &game);
void show_stats(GameEngine &game);

int main() {
    init();      
    int selectedMode = welcome();  // 获取玩家选择的模式

    // Turn on all red LEDs initially
    for (int i = 0; i < 3; i++) {
        redLEDs[i] = 1;
    }

    GameEngine game(lcd, joystick, buttonA, buttonB, selectedMode, redLEDs, greenLED);
    game.init();
    game.run();

    game_over(game); 
}

void init() {
    lcd.init(LPH7366_1);
    lcd.setContrast(0.5);
    joystick.init(); 
}

int welcome() {
    int selectedMode = -1;
    
    while (selectedMode == -1) {
        lcd.clear();
        lcd.printString("Aim Club", 0, 0);   
        lcd.printString("Press ButtonC", 0, 2); 
        lcd.printString("Timed Mode", 0, 3);
        lcd.printString("Press ButtonD", 0, 4);
        lcd.printString("Endless Mode", 0, 5);
        lcd.refresh();

        // 检查按钮状态
        if (buttonC.read() == 1) {
            selectedMode = 0; // Timed Mode
            while (buttonC.read() == 0) {
                ThisThread::sleep_for(20ms);
            }
        } 
        if (buttonD.read() == 1) {
            selectedMode = 1; // Endless Mode
            while (buttonD.read() == 0) {
                ThisThread::sleep_for(20ms);
            }
        }

        ThisThread::sleep_for(100ms);
    }

    // 确认模式 + 等待蓝色按钮
    lcd.clear();
    if (selectedMode == 0) {
        lcd.printString("Timed Mode", 0, 0);
    } else {
        lcd.printString("Endless Mode", 0, 0);
    }
    lcd.printString("Press", 0, 3);
    lcd.printString("Blue Button", 0, 4);
    lcd.refresh();

    // 等待蓝色按钮按下
    while (buttonA.read() == 1) {
        ThisThread::sleep_for(50ms);
    }

    // 等待释放
    while (buttonA.read() == 0) {
        ThisThread::sleep_for(20ms);
    }

    return selectedMode;
}

void game_over(GameEngine &game) {
    // 闪烁三次"Game Over"
    for (int i = 0; i < 3; i++) {
        lcd.clear();
        lcd.printString("  Game Over ", 0, 2);  
        lcd.refresh();
        ThisThread::sleep_for(500ms);
        lcd.clear();
        lcd.refresh();
        ThisThread::sleep_for(500ms);
    }
    
    // 显示统计信息
    show_stats(game);
}

void show_stats(GameEngine &game) {
    int score, circleCount, squareCount, triangleCount;
    game.getStats(score, circleCount, squareCount, triangleCount);
    
    while (1) {
        lcd.clear();
        
        // 显示分数
        char scoreStr[20];
        sprintf(scoreStr, "Score: %d", score);
        lcd.printString(scoreStr, 0, 0);
        
        // 显示圆形击中数
        char circleStr[20];
        sprintf(circleStr, "Circle: %d", circleCount);
        lcd.printString(circleStr, 0, 1);
        
        // 显示正方形击中数
        char squareStr[20];
        sprintf(squareStr, "Square: %d", squareCount);
        lcd.printString(squareStr, 0, 2);
        
        // 显示三角形击中数
        char triangleStr[20];
        sprintf(triangleStr, "Triangle: %d", triangleCount);
        lcd.printString(triangleStr, 0, 3);
        
        lcd.refresh();
        ThisThread::sleep_for(100ms);
    }
}
