#ifndef BREAKOUTDISPLAY_H_
#define BREAKOUTDISPLAY_H_

#include <stdbool.h>
#include <stdint.h>


// Given coordinates from the touch pad, computes the region number.

// These are the definitions for the regions.
#define SIMON_DISPLAY_REGION_0 0
#define SIMON_DISPLAY_REGION_1 1
#define SIMON_DISPLAY_REGION_2 2
#define SIMON_DISPLAY_REGION_3 3

// Number of touchable regions.
#define SIMON_DISPLAY_REGION_COUNT 4

// Passed to simonDisplay_drawButton to indicate drawing or erasing
#define SIMON_DISPLAY_DRAW 0
#define SIMON_DISPLAY_ERASE 1

int8_t breakoutDisplay_computeRegionNumber(int16_t x, int16_t y);

// Draws a colored "button" that the user can touch.
// The colored button is centered in the region but does not fill the region.
// If erase argument is true, draws the button as black background to erase it.
void breakoutDisplay_drawPaddle(uint8_t regionNumber, bool erase);

// Convenience function that draws all of the buttons.
void breakoutDisplay_drawNewTiles();

// Convenience function that erases all of the buttons.
void breakoutDisplay_eraseAllTiles();

// Draws a bigger square that completely fills the region.
// If the erase argument is true, it draws the square as black background to
// "erase" it.
void breakoutDisplay_drawSquare(uint8_t regionNo, bool erase);

#endif /* SIMONDISPLAY_H_ */