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




int8_t breakoutDisplay_computeRegionNumber(int16_t x, int16_t y);

// Draws a colored "button" that the user can touch.
// The colored button is centered in the region but does not fill the region.
// If erase argument is true, draws the button as black background to erase it.
void breakoutDisplay_drawPaddle(uint8_t regionNumber, bool erase);

//a function that draws an indivicual tile for use in the game
//it takes in the x and y coordinates and a number of the color for 
void breakoutDisplay_drawTile(uint16_t xCoord, uint16_t yCoord, uint32_t colorCode)

// a fucntion that draws up the tiles for the start of the level.
void breakoutDisplay_drawNewTiles();

// Convenience function that erases the entire tile area.
void breakoutDisplay_eraseAllTiles();

//this fucntion is to draw the ball that will be boucnign around the s
void breakoutDisplay_drawBall(uint16_t xCoord, uint16_t yCoord, bool erase);



#endif /* SIMONDISPLAY_H_ */