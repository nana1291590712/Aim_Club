#ifndef TARGET_H
#define TARGET_H

#include "mbed.h"
#include "N5110.h"
#include "Utils.h"

class Target {
public:
    /**
     * @brief Constructor for Target class
     * @param lcd Reference to N5110 LCD object
     */
    Target(N5110 &lcd);
    
    /**
     * @brief Initializes the target system
     */
    void init();
    
    /**
     * @brief Generates a new random target at a random position
     */
    void generate();
    
    /**
     * @brief Draws the current target on the LCD screen
     */
    void draw();
    
    /**
     * @brief Checks if given position hits the target
     * @param pos Position to check (x,y coordinates)
     * @return True if position hits the target, false otherwise
     */
    bool checkHit(Position2D pos);
    
    /**
     * @brief Gets the current target's position
     * @return Position2D struct with target coordinates
     */
    Position2D getPosition();
    
    /**
     * @brief Gets the current target's size
     * @return Size of the target
     */
    int getSize();

private:
    N5110 &_lcd;
    Position2D _position;
    int _type;  // 0 = square, 1 = triangle, 2 = circle
    int _size;
    
    void drawSquare();
    void drawTriangle();
    void drawCircle();
    void randomizePosition();
};

#endif
