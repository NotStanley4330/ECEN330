#ifndef BREAKOUTDISPLAY_H_
#define BREAKOUTDISPLAY_H_

#include <stdbool.h>
#include <stdint.h>



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
#define PADDLE_WIDTH 64
#define PADDLE_HEIGHT 6
#define BALL_WIDTH_HEIGHT 8
#define TILE_WIDTH 32
#define TILE_HEIGHT 6


//starting coordinates for the paddle
#define PADDLE_INIT_X_COORD ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) - (PADDLE_WIDTH / 2))
#define PADDLE_INIT_Y_COORD (DISPLAY_HEIGHT - 10)

//starting coords for the ball
#define BALL_INIT_X_COORD ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER))
#define BALL_INIT_Y_COORD (PADDLE_INIT_Y_COORD - 10)

//Score display locations
#define SCORE_TEXT_X_COORD ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + 64)
#define SCORE_TEXT_Y_COORD 3
#define SCORE_X_COORD (SCORE_TEXT_X_COORD + 40)
#define SCORE_Y_COORD SCORE_TEXT_Y_COORD

//first cords we can draw a tile at
#define TILE_FIRST_X_COORD 6
#define TILE_FIRST_Y_COORD 16

//color number deifinitons
#define COLOR_NUM_BLUE 0
#define COLOR_NUM_RED 1
#define COLOR_NUM_YELLOW 2
#define COLOR_NUM_GREEN 4
#define COLOR_NUM_ORANGE 5
#define COLOR_NUM_PURPLE 6
#define COLOR_NUM_CYAN 7

//custom color defr
#define DISPLAY_ORANGE 0xFBA0
#define DISPLAY_TEAL 0x0737
#define DISPLAY_PURPLE 0x8155




int8_t breakoutDisplay_computeRegionNumber(int16_t x, int16_t y);

uint16_t breakoutDisplay_assignColorCode(uint8_t colorNum);

void breakoutDisplay_drawScore(uint32_t score, bool erase);

// Draws a colored "button" that the user can touch.
// The colored button is centered in the region but does not fill the region.
// If erase argument is true, draws the button as black background to erase it.
void breakoutDisplay_drawPaddle(int16_t xCoord, int16_t yCoord, bool erase);

//a function that draws an indivicual tile for use in the game
//it takes in the x and y coordinates and a number of the color for 
void breakoutDisplay_drawTile(int16_t xCoord, int16_t yCoord, uint16_t colorCode, bool erase);

// a fucntion that draws up the tiles for the start of the level.
void breakoutDisplay_drawNewTiles();

// Convenience function that erases the entire tile area.
void breakoutDisplay_eraseAllTiles();

//this fucntion is to draw the ball that will be boucnign around the s
void breakoutDisplay_drawBall(int16_t xCoord, int16_t yCoord, bool erase);



#endif /* SIMONDISPLAY_H_ */