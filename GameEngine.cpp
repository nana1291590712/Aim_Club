
#include "GameEngine.h"

GameEngine::GameEngine(N5110 &lcd, Joystick &joystick, DigitalIn &buttonA, InterruptIn &buttonB, int mode, DigitalOut redLEDs[], DigitalOut &greenLED)
    : _lcd(lcd), _joystick(joystick), _buttonA(buttonA), _buttonB(buttonB),
      _aim(lcd, joystick, buttonB), _target(lcd), _specialTarget(lcd), _mode(mode), _redLEDs(redLEDs), _greenLED(greenLED) {
    _gameOver = false;
    _timeLimit = 50.0f;
    _lives = 3;

    _circleHits = 0;
    _squareHits = 0;
    _triangleHits = 0;

    _freezeTargets = false;
    _slowAim = false;
    _effectMessageShown = false;
    _nextSpecialTime = 5.0f;
}

void GameEngine::init() {
    _aim.init();
    _target.init();
    _specialTarget.init();

    _gameTimer.start();
    _target.generate();
    updateLivesDisplay();
}

void GameEngine::updateLivesDisplay() {
    for (int i = 0; i < 3; i++) {
        _redLEDs[i] = (i < _lives) ? 1 : 0;
    }
}

void GameEngine::loseLife() {
    if (_lives > 0) {
        _lives--;
        for (int i = 0; i < 2; i++) {
            _redLEDs[_lives] = 0;
            ThisThread::sleep_for(125ms);
            _redLEDs[_lives] = 1;
            ThisThread::sleep_for(125ms);
        }
        updateLivesDisplay();
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

    if (!_freezeTargets) {
        _target.move();
    }

    float elapsed = std::chrono::duration<float>(_gameTimer.elapsed_time()).count();

    if (elapsed >= _nextSpecialTime && !_specialTarget.isActive()) {
         _specialTarget.generate(_mode);  // 传入模式
         _nextSpecialTime += 10.0f;
         _effectMessageShown = false;
}


    _specialTarget.update();

    if (_aim.isFired()) {
        Position2D aimPos = _aim.getPosition();
        _aim.drawExplosionAnimation();

        if (_specialTarget.isActive() && _specialTarget.checkHit(aimPos)) {
            flashGreenLED();
            applyEffect(_specialTarget.getEffect());
        } else if (_target.checkHit(aimPos)) {
            flashGreenLED();
            handleHit(aimPos);
        } else {
            loseLife();
        }
    }

    if (_mode == 0 && elapsed >= _timeLimit) {
        _gameOver = true;
    }

    if (_slowAim && _effectTimer.elapsed_time() >= 5000ms) {
        _slowAim = false;
        _aim.setSpeedMultiplier(1.0f);
    }
}

void GameEngine::draw() {
    _lcd.clear();
    _target.draw();
    _specialTarget.draw();
    _aim.draw();
    if (_effectMessageShown) {
        drawEffectMessage();
    }
    drawTimeLeft();
    _lcd.refresh();
}


void GameEngine::flashGreenLED() {
    for (int i = 0; i < 2; i++) {
        _greenLED = 1;
        ThisThread::sleep_for(125ms);
        _greenLED = 0;
        ThisThread::sleep_for(125ms);
    }
}

void GameEngine::getStats(int &score, int &circleCount, int &squareCount, int &triangleCount) {
    circleCount = _circleHits;
    squareCount = _squareHits;
    triangleCount = _triangleHits;
    score = _circleHits * 1 + _squareHits * 2 + _triangleHits * 3;
}

void GameEngine::handleHit(Position2D aimPos) {
    if (_target.checkHit(aimPos)) {
        int type = _target.getType();
        switch (type) {
            case 0: _squareHits++; break;
            case 1: _triangleHits++; break;
            case 2: _circleHits++; break;
        }
        _freezeTargets = false;  // 重置冻结标志
        _target.generate();      // 生成新目标
    }
}

void GameEngine::applyEffect(SpecialEffect effect) {
    switch (effect) {
        case SpecialEffect::AddTime:
            _timeLimit += 15.0f;
            break;
        case SpecialEffect::FreezeTargets:
            _freezeTargets = true;
            break;
        case SpecialEffect::SlowAim:
            _slowAim = true;
            _aim.setSpeedMultiplier(0.1f);
            _effectTimer.reset();
            _effectTimer.start();
            break;
        case SpecialEffect::AddLife:
            if (_lives < 3) {
                _redLEDs[_lives] = 1;
                _lives++;
            }
            break;
    }
    _effectMessageShown = true;
}

void GameEngine::drawEffectMessage() {
    if (_effectMessageShown) {
        static Timer blinkTimer;
        static int blinkCount = 0;
        static bool messageVisible = true;
        static bool timerStarted = false;

        if (!timerStarted) {
            blinkTimer.start();
            timerStarted = true;
        }

        if (blinkTimer.elapsed_time() > 500ms) {
            messageVisible = !messageVisible;
            blinkTimer.reset();
            blinkCount++;

            if (blinkCount >= 6) {
                _effectMessageShown = false;
                blinkCount = 0;
                messageVisible = true;
                timerStarted = false;
            }
        }

        if (messageVisible) {
            switch (_specialTarget.getEffect()) {
                case SpecialEffect::AddTime:
                    _lcd.printString("+15s!", 30, 0);
                    break;
                case SpecialEffect::AddLife:
                    _lcd.printString("LIVES+1!", 24, 0);
                    break;
                case SpecialEffect::FreezeTargets:
                    _lcd.printString("FREEZE!", 24, 0);
                    break;
                case SpecialEffect::SlowAim:
                    _lcd.printString("STOP!", 32, 0);
                    break;
                default:
                    break;  // 安全兜底
            }
        }
    }
}

void GameEngine::drawTimeLeft() {
    if (_mode == 0) {  // TIMED 模式
        int secondsLeft = static_cast<int>(_timeLimit - std::chrono::duration_cast<std::chrono::seconds>(_gameTimer.elapsed_time()).count());
        if (secondsLeft < 0) secondsLeft = 0;

        char buffer[14];
        sprintf(buffer, "%2ds", secondsLeft);
        _lcd.printString(buffer, 66, 0);  // 右上角显示时间
    }
}
