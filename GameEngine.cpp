#include "GameEngine.h"

// In GameEngine.cpp
GameEngine::GameEngine(N5110 &lcd, Joystick &joystick, DigitalIn &buttonA, InterruptIn &buttonB)
    : _lcd(lcd), _joystick(joystick), _buttonA(buttonA), _buttonB(buttonB), 
      _aim(lcd, joystick, buttonB), _target(lcd) {
    _gameOver = false;
}

void GameEngine::init() {
    _aim.init();
    _target.init();
    _target.generate();
}

void GameEngine::update() {
    _aim.update();

    if (_aim.isFired()) {
        _lcd.printString("  FIRE!", 24, 2);
        _lcd.refresh();
        ThisThread::sleep_for(150ms);
        checkGameState();
    }
}

void GameEngine::run() {
    while (!_gameOver) {
        update();
        draw();
        ThisThread::sleep_for(50ms);
    }
}

void GameEngine::checkGameState() {
    Position2D center = _aim.getPosition();

    if (_target.checkHit(center)) {
        _target.generate();
    } else {
        _gameOver = true;
    }
}

void GameEngine::draw() {
    _lcd.clear();
    _target.draw();
    _aim.draw();
    _lcd.refresh();
}