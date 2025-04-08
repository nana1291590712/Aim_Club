///////////// includes /////////////////////
#include "mbed.h"
#include "Joystick.h"
#include "N5110.h"  
#include "Utils.h"
#include "GameEngine.h"
#include "Aim.h"
#include "Target.h"

///////////// objects ///////////////////
N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);
Joystick joystick(PC_1, PC_0);
DigitalIn buttonA(BUTTON1);
InterruptIn buttonB(PC_5);  // 新增的按钮 B

///////////// prototypes ///////////////
void init();
void render();
void welcome();
void game_over();

int main() {
    init();      // initialise devices and objects
    welcome();   // waiting for the user to start 
    
    // Create and run game engine
    GameEngine game(lcd, joystick, buttonA, buttonB);  // 传递 buttonA 和 buttonB
    game.init();
    game.run();
    
    game_over();
}

void init() {
    lcd.init(LPH7366_1);
    lcd.setContrast(0.5);
    joystick.init();
}

void render() {  // clear screen, re-draw and refresh
    lcd.clear();  
    lcd.refresh();
}

void welcome() { // splash screen
    lcd.printString("Game Start!",0,1);  
    lcd.printString("Press Nucleo",0,3);
    lcd.printString("Blue button",0,4);
    lcd.refresh();

    while (buttonA.read() == 1) {
        ThisThread::sleep_for(100ms);
    }
}

void game_over() { // splash screen 
    while (1) {
        lcd.clear();
        lcd.printString("  Game Over ",0,2);  
        lcd.refresh();
        ThisThread::sleep_for(250ms);
        lcd.clear();
        lcd.refresh();
        ThisThread::sleep_for(250ms);
    }
}
