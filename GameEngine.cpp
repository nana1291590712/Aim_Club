#include "GameEngine.h"

GameEngine::GameEngine(N5110 &lcd, Joystick &joystick, DigitalIn &buttonA, InterruptIn &buttonB, int mode, DigitalOut redLEDs[], DigitalOut &greenLED)
    : _lcd(lcd), _joystick(joystick), _buttonA(buttonA), _buttonB(buttonB),
      _aim(lcd, joystick, buttonB), _target(lcd), _mode(mode), _redLEDs(redLEDs), _greenLED(greenLED) {
    _gameOver = false;
    _timeLimit = 30.0f; // 默认限时
    _lives = 3;         // 初始生命值
    _squareHits = 0;    // 初始化正方形命中次数
    _triangleHits = 0;  // 初始化三角形命中次数
    _circleHits = 0;    // 初始化圆形命中次数
}

void GameEngine::init() {
    _aim.init();
    _target.init();
    _gameTimer.start();
    _target.generate();       // 初始化生成一个目标
    updateLivesDisplay();     // 初始化LED显示
}

void GameEngine::updateLivesDisplay() {
    for (int i = 0; i < 3; i++) {
        _redLEDs[i] = (i < _lives) ? 1 : 0; // 亮的LED表示剩余生命
    }
}

void GameEngine::loseLife() {
    if (_lives > 0) {
        _lives--;

        // 先让即将熄灭的 LED 闪烁两次
        for (int i = 0; i < 2; i++) {
            _redLEDs[_lives] = 0;
            ThisThread::sleep_for(125ms);
            _redLEDs[_lives] = 1;
            ThisThread::sleep_for(125ms);
        }

        updateLivesDisplay();  // 最终更新 LED 显示状态
    }

    if (_lives <= 0) {
        _gameOver = true;
    }
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
    _target.move();

    if (_aim.isFired()) {
        checkGameState();
    }

    // 如果是 timed 模式，检查时间限制
    if (_mode == 0 && std::chrono::duration<float>(_gameTimer.elapsed_time()).count() >= _timeLimit) {
        _gameOver = true;
    }
}

void GameEngine::checkGameState() {
    Position2D aimPos = _aim.getPosition();

    _aim.drawExplosionAnimation();

    if (_target.checkHit(aimPos)) {
        flashGreenLED();
        handleHit(aimPos);  // 先处理命中逻辑
        _target.generate(); // 然后生成新目标
    } else {
        loseLife();         // 未命中才扣血
    }
}

void GameEngine::flashGreenLED() {
    for (int i = 0; i < 2; i++) {
        _greenLED = 1;
        ThisThread::sleep_for(125ms);
        _greenLED = 0;
        ThisThread::sleep_for(125ms);
    }
}

void GameEngine::draw() {
    _lcd.clear();
    _target.draw();
    _aim.draw();
    _lcd.refresh();
}

void GameEngine::getStats(int &score, int &circleCount, int &squareCount, int &triangleCount) {
    circleCount = _circleHits;
    squareCount = _squareHits;
    triangleCount = _triangleHits;
    score = _circleHits * 1 + _squareHits * 2 + _triangleHits * 3;
}

void GameEngine::handleHit(Position2D aimPos) {
    if (_target.checkHit(aimPos)) {  // 再次确认命中
        int type = _target.getType();
        
        switch (type) {
            case 0: _squareHits++; break;
            case 1: _triangleHits++; break;
            case 2: _circleHits++; break;
        }
    }
}
