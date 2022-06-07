

#include <stdbool.h>
#include <stdint.h>
#include "breakoutDisplay.h"
#include "utils.h"
#include "display.h"
#include <stdio.h>


// These are the definitions for the regions.
//region zero will be the left side of the screen and region one the right
/*
-------------------------------------
                |
                |
    Region  0   |      Region 1
                |
                |
                |
                |
-------------------------------------
We only care which side they press on to move the paddle left and right, not anything else
*/
#define BREAKOUT_DISPLAY_REGION_0 0
#define BREAKOUT_DISPLAY_REGION_1 1

// a value used for dividing screen length and height by 2
#define SCREEN_REGION_DIVIDER 2


// Number of touchable regions.
#define BREAKOUT_DISPLAY_REGION_COUNT 2

// Passed to simonDisplay_drawButton to indicate drawing or erasing
#define BREAKOUT_DISPLAY_DRAW 0
#define BREAKOUT_DISPLAY_ERASE 1

//size definitions for everything
#define PADDLE_WIDTH 16
#define PADDLE_HEIGHT 3
#define BALL_WIDTH_HEIGHT 4
#define TILE_WIDTH 8
#define TILE_HEIGHT 3


//starting coordinates for the paddle
#define PADDLE_INIT_X_COORD ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) - (PADDLE_WIDTH / 2))
#define PADDLE_INIT_Y_COORD (DISPLAY_HEIGHT - 3)

//starting coords for the ball
#define BALL_INIT_X_COORD (DISPLAY_WIDTH - (BALL_WIDTH_HEIGHT / 2))
#define BALL_INIT_Y_COORD (PADDLE_INIT_Y_COORD - BALL_INIT_X_COORD -1)

struct ballProperties
{
    uint8_t xVelocity;
    uint8_t yVelocity;
    uint8_t xPosition;
    uint8_t yPosition;
}




//this functiont akes the coordinates and computes the region of the screen that was touched
int8_t breakoutDisplay_computeRegionNumber(int16_t x, int16_t y)
{
    //if the touch was on the left half of the screen
    if (x < DISPLAY_WIDTH / SCREEN_REGION_DIVIDER)
    {
        //give us the left region number
        return BREAKOUT_DISPLAY_REGION_0;
    }
    else//otherwise the the touch was on the right side
    {
            //give us the right reigon number
            return BREAKOUT_DISPLAY_REGION_1
    }
}

// Draws a colored "button" that the user can touch.
// The colored button is centered in the region but does not fill the region.
// If erase argument is true, draws the button as black background to erase it.
void breakoutDisplay_drawPaddle(uint16_t xCoord, uint16_t yCoord, bool erase)
{
    display_fillRect(xCoord, yCoord, PADDLE_WIDTH, PADDLE_HEIGHT, DISPLAY_WHITE);
}

//a function that draws an indivicual tile for use in the game
//it takes in the x and y coordinates and a number of the color for 
void breakoutDisplay_drawTile(uint16_t xCoord, uint16_t yCoord, uint32_t colorCode, bool erase)
{

}

// a fucntion that draws up the tiles for the start of the level.
void breakoutDisplay_drawNewTiles();

// Convenience function that erases the entire tile area.
void breakoutDisplay_eraseAllTiles();

//this fucntion is to draw the ball that will be boucnign around the s
void breakoutDisplay_drawBall(uint16_t xCoord, uint16_t yCoord, bool erase);