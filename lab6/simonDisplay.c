/*
    File: simonDisplay.c
    Author: Michael Norris
    Date: 5/27/2022
    Purpose: Handles the drawing and erasing of buttons and enlarged squares




*/
#include "simonDisplay.h"
#include "display.h"
#include "utils.h"
#include <stdio.h>

// custom defines

// region defines
// value for the top left region
#define REGION_0 0
// value for the top right region
#define REGION_1 1
// value for the bottom left region
#define REGION_2 2
// value for the bottom right region
#define REGION_3 3
// a value used for dividing screen length and height by 2
#define SCREEN_REGION_DIVIDER 2

// button defs
// we want the squares to be about centered in each region
// so offset them by 50 on x and 30 on y from the center lines
#define BUTTON_X_LOCATION_OFFSET 50
#define BUTTON_Y_LOCATION_OFFSET 30
// square offsets should be about half of button offsets
#define SQUARE_X_LOCATION_OFFSET 25
#define SQUARE_Y_LOCATION_OFFSET 15
// each button is square with sides about 60 units in length
#define BUTTON_SQUARE_SIDE_LENGTH 60
// enlarged square can just be double size
#define ENLARGED_SQUARE_SIDE_LENGTH 120

// this function will recieve the x and y values from
// buttonHandler_getRegionNumber() and use those to compute the region touched
int8_t simonDisplay_computeRegionNumber(int16_t x, int16_t y) {
  // find if it is in the top half first
  if (y < DISPLAY_HEIGHT / SCREEN_REGION_DIVIDER) {
    // now check if in left or right half of the top
    if (x < DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) {
      // top left is region 0
      return REGION_0;
    } else // otherwise its in the top right
    {
      // return region 1
      return REGION_1;
    }
  } else // if its not up top its in the bottom
  {
    // now check if in left or right half of the bottom
    if (x < DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) {
      // this would be the bottom left, region 2
      return REGION_2;
    } else // bottom right
    {
      // return region 3
      return REGION_3;
    }
  }
}

// Draws a colored "button" that the user can touch.
// The colored button is centered in the region but does not fill the region.
// If erase argument is true, draws the button as black background to erase it.
void simonDisplay_drawButton(uint8_t regionNumber, bool erase) {
  if (!erase) // if we arnet erasing the button
  {
    switch (regionNumber) // check the paassed in region number to determine
                          // which button to draw
    {
    // just draw a square of a certain color and locaio
    case REGION_0:
      display_fillRect(BUTTON_X_LOCATION_OFFSET, BUTTON_Y_LOCATION_OFFSET,
                       BUTTON_SQUARE_SIDE_LENGTH, BUTTON_SQUARE_SIDE_LENGTH,
                       DISPLAY_RED);
      break;
    case REGION_1:
      display_fillRect(
          ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + BUTTON_X_LOCATION_OFFSET),
          BUTTON_Y_LOCATION_OFFSET, BUTTON_SQUARE_SIDE_LENGTH,
          BUTTON_SQUARE_SIDE_LENGTH, DISPLAY_YELLOW);
      break;
    case REGION_2:
      display_fillRect(
          BUTTON_X_LOCATION_OFFSET,
          ((DISPLAY_HEIGHT / SCREEN_REGION_DIVIDER) + BUTTON_Y_LOCATION_OFFSET),
          BUTTON_SQUARE_SIDE_LENGTH, BUTTON_SQUARE_SIDE_LENGTH, DISPLAY_BLUE);
      break;
    case REGION_3:
      display_fillRect(
          ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + BUTTON_X_LOCATION_OFFSET),
          ((DISPLAY_HEIGHT / SCREEN_REGION_DIVIDER) + BUTTON_Y_LOCATION_OFFSET),
          BUTTON_SQUARE_SIDE_LENGTH, BUTTON_SQUARE_SIDE_LENGTH, DISPLAY_GREEN);
      break;
    }
  } else // we are erasing the button
  {
    switch (regionNumber) // check the paassed in region number to determine
                          // which button to draw
    {
    // just draw a square of a certain color and locaio
    case REGION_0:
      display_fillRect(BUTTON_X_LOCATION_OFFSET, BUTTON_Y_LOCATION_OFFSET,
                       BUTTON_SQUARE_SIDE_LENGTH, BUTTON_SQUARE_SIDE_LENGTH,
                       DISPLAY_BLACK);
      break;
    case REGION_1:
      display_fillRect(
          ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + BUTTON_X_LOCATION_OFFSET),
          BUTTON_Y_LOCATION_OFFSET, BUTTON_SQUARE_SIDE_LENGTH,
          BUTTON_SQUARE_SIDE_LENGTH, DISPLAY_BLACK);
      break;
    case REGION_2:
      display_fillRect(
          BUTTON_X_LOCATION_OFFSET,
          ((DISPLAY_HEIGHT / SCREEN_REGION_DIVIDER) + BUTTON_Y_LOCATION_OFFSET),
          BUTTON_SQUARE_SIDE_LENGTH, BUTTON_SQUARE_SIDE_LENGTH, DISPLAY_BLACK);
      break;
    case REGION_3:
      display_fillRect(
          ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + BUTTON_X_LOCATION_OFFSET),
          ((DISPLAY_HEIGHT / SCREEN_REGION_DIVIDER) + BUTTON_Y_LOCATION_OFFSET),
          BUTTON_SQUARE_SIDE_LENGTH, BUTTON_SQUARE_SIDE_LENGTH, DISPLAY_BLACK);
      break;
    }
  }
}

// Convenience function that draws all of the buttons.
void simonDisplay_drawAllButtons() {
  // simply draw all 4 buttons
  simonDisplay_drawButton(REGION_0, 0);
  simonDisplay_drawButton(REGION_1, 0);
  simonDisplay_drawButton(REGION_2, 0);
  simonDisplay_drawButton(REGION_3, 0);
}

// Convenience function that erases all of the buttons.
void simonDisplay_eraseAllButtons() {
  // just erase everything
  simonDisplay_drawButton(REGION_0, 1);
  simonDisplay_drawButton(REGION_1, 1);
  simonDisplay_drawButton(REGION_2, 1);
  simonDisplay_drawButton(REGION_3, 1);
}

// Draws a bigger square that completely fills the region.
// If the erase argument is true, it draws the square as black background to
// "erase" it.
void simonDisplay_drawSquare(uint8_t regionNo, bool erase) {
  if (!erase) // if we arnet erasing the square
  {
    switch (regionNo) // check the paassed in region number to determine which
                      // square to draw
    {
    // just draw a square of a certain color and location
    case REGION_0:
      display_fillRect(SQUARE_X_LOCATION_OFFSET, SQUARE_Y_LOCATION_OFFSET,
                       ENLARGED_SQUARE_SIDE_LENGTH, ENLARGED_SQUARE_SIDE_LENGTH,
                       DISPLAY_RED);
      break;
    case REGION_1:
      display_fillRect(
          ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + SQUARE_X_LOCATION_OFFSET),
          SQUARE_Y_LOCATION_OFFSET, ENLARGED_SQUARE_SIDE_LENGTH,
          ENLARGED_SQUARE_SIDE_LENGTH, DISPLAY_YELLOW);
      break;
    case REGION_2:
      display_fillRect(
          SQUARE_X_LOCATION_OFFSET,
          ((DISPLAY_HEIGHT / SCREEN_REGION_DIVIDER) + SQUARE_Y_LOCATION_OFFSET),
          ENLARGED_SQUARE_SIDE_LENGTH, ENLARGED_SQUARE_SIDE_LENGTH,
          DISPLAY_BLUE);
      break;
    case REGION_3:
      display_fillRect(
          ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + SQUARE_X_LOCATION_OFFSET),
          ((DISPLAY_HEIGHT / SCREEN_REGION_DIVIDER) + SQUARE_Y_LOCATION_OFFSET),
          ENLARGED_SQUARE_SIDE_LENGTH, ENLARGED_SQUARE_SIDE_LENGTH,
          DISPLAY_GREEN);
      break;
    }
  } else // we are erasing the quare
  {
    switch (regionNo) // check the paassed in region number to determine which
                      // square to erase
    {
    // just erase the given square
    case REGION_0:
      display_fillRect(SQUARE_X_LOCATION_OFFSET, SQUARE_Y_LOCATION_OFFSET,
                       ENLARGED_SQUARE_SIDE_LENGTH, ENLARGED_SQUARE_SIDE_LENGTH,
                       DISPLAY_BLACK);
      break;
    case REGION_1:
      display_fillRect(
          ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + SQUARE_X_LOCATION_OFFSET),
          SQUARE_Y_LOCATION_OFFSET, ENLARGED_SQUARE_SIDE_LENGTH,
          ENLARGED_SQUARE_SIDE_LENGTH, DISPLAY_BLACK);
      break;
    case REGION_2:
      display_fillRect(
          SQUARE_X_LOCATION_OFFSET,
          ((DISPLAY_HEIGHT / SCREEN_REGION_DIVIDER) + SQUARE_Y_LOCATION_OFFSET),
          ENLARGED_SQUARE_SIDE_LENGTH, ENLARGED_SQUARE_SIDE_LENGTH,
          DISPLAY_BLACK);
      break;
    case REGION_3:
      display_fillRect(
          ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + SQUARE_X_LOCATION_OFFSET),
          ((DISPLAY_HEIGHT / SCREEN_REGION_DIVIDER) + SQUARE_Y_LOCATION_OFFSET),
          ENLARGED_SQUARE_SIDE_LENGTH, ENLARGED_SQUARE_SIDE_LENGTH,
          DISPLAY_BLACK);
      break;
    }
  }
}
