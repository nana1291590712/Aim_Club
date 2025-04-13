#include "GameEngine.h"

GameEngine::GameEngine(N5110 &lcd, Joystick &joystick, DigitalIn &buttonA, InterruptIn &buttonB)
    : _lcd(lcd), _joystick(joystick), _buttonA(buttonA), _buttonB(buttonB),
      _aim(lcd, joystick, buttonB), _target(lcd) {
    _gameOver = false;
    _mode = 0;          // 默认 timed 模式
    _timeLimit = 30.0f; // 30 秒限时
}

void GameEngine::init() {
    _aim.init();
    _target.init();
    _gameTimer.start();
    _target.generate();  // 初始化生成一个目标
}

void GameEngine::run() {
    while (!_gameOver) {
        update();
        draw();
        ThisThread::sleep_for(50ms);
    }
}

void GameEngine::update() {
    _aim.update();
    _target.move();  // 缓慢移动

    if (_aim.isFired()) {
        checkGameState();
    }

    // 修正 deprecated read() 用法
    if (_mode == 0 && std::chrono::duration<float>(_gameTimer.elapsed_time()).count() >= _timeLimit) {
        _gameOver = true;
    }
}

void GameEngine::checkGameState() {
    Position2D center = _aim.getPosition();

    if (_target.checkHit(center)) {
        _target.generate();  // 命中后生成新目标
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
