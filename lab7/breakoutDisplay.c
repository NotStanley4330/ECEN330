

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
#define PADDLE_WIDTH 64
#define PADDLE_HEIGHT 6
#define BALL_WIDTH_HEIGHT 8
#define TILE_WIDTH 32
#define TILE_HEIGHT 6
#define TILE_SPACER_WIDTH 3 //this is a little space between the tiles


//starting coordinates for the paddle
#define PADDLE_INIT_X_COORD ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) - (PADDLE_WIDTH / 2))
#define PADDLE_INIT_Y_COORD (DISPLAY_HEIGHT - 10)

//starting coords for the ball
#define BALL_INIT_X_COORD ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER))
#define BALL_INIT_Y_COORD (PADDLE_INIT_Y_COORD - 10)

//first cords we can draw a tile at
#define TILE_FIRST_X_COORD 6
#define TILE_FIRST_Y_COORD 16

//Score display locations
#define SCORE_TEXT_X_COORD ((DISPLAY_WIDTH / SCREEN_REGION_DIVIDER) + 64)
#define SCORE_TEXT_Y_COORD 3
#define SCORE_X_COORD (SCORE_TEXT_X_COORD + 40)
#define SCORE_Y_COORD SCORE_TEXT_Y_COORD

//score string
#define SCORE_STRING "SCORE: "

//color number deifinitons
#define COLOR_NUM_BLUE 0
#define COLOR_NUM_RED 1
#define COLOR_NUM_YELLOW 2
#define COLOR_NUM_GREEN 4
#define COLOR_NUM_ORANGE 5
#define COLOR_NUM_PURPLE 6
#define COLOR_NUM_CYAN 7

//custom color def
#define DISPLAY_ORANGE 0xFBA0
#define DISPLAY_PURPLE 0x8155
#define DISPLAY_TEAL 0x0737


struct ballProperties
{
    uint8_t xVelocity;
    uint8_t yVelocity;
    uint8_t xPosition;
    uint8_t yPosition;
};







void breakoutDisplay_drawScore(uint32_t score)
{
    char scoreString[20] = "";
    
    display_setCursor(SCORE_TEXT_X_COORD, SCORE_TEXT_Y_COORD);
    display_setTextColor(DISPLAY_WHITE);
    display_setTextSize(1);
    display_println(SCORE_STRING);
    display_setCursor(SCORE_X_COORD, SCORE_Y_COORD);
    sprintf(scoreString, "%i", score);
    display_println(scoreString);

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
        return BREAKOUT_DISPLAY_REGION_1;
    }
}

//this function takes a number and returns a 16 bit color code based on the given number
uint16_t breakoutDisplay_assignColorCode(uint8_t colorNum)
{
    //switch statment to take the given integer and return a color based on that
    switch(colorNum)
    {
        case COLOR_NUM_BLUE:
            return DISPLAY_BLUE;
            break;
        case  COLOR_NUM_RED:
            return DISPLAY_RED;
            break;
        case COLOR_NUM_YELLOW:
            return DISPLAY_YELLOW;
            break;
        case COLOR_NUM_GREEN:
            return DISPLAY_GREEN;
            break;
        case COLOR_NUM_ORANGE:
            return DISPLAY_ORANGE;
            break;
        case COLOR_NUM_PURPLE:
            return DISPLAY_PURPLE;
            break;
        case COLOR_NUM_CYAN:
            return DISPLAY_CYAN;
            break;
    }
}

// Draws a colored "button" that the user can touch.
// The colored button is centered in the region but does not fill the region.
// If erase argument is true, draws the button as black background to erase it.
void breakoutDisplay_drawPaddle(int16_t xCoord, int16_t yCoord, bool erase)
{
    //if we are not erasing it
    if(!erase)
    {
        //draw the paddle in white
        display_fillRect(xCoord, yCoord, PADDLE_WIDTH, PADDLE_HEIGHT, DISPLAY_TEAL);
    }
    else//if we are erasing
    {
        //get rid of the thang
        display_fillRect(xCoord, yCoord, PADDLE_WIDTH, PADDLE_HEIGHT, DISPLAY_BLACK);
    }
}

//a function that draws an indivicual tile for use in the game
//it takes in the x and y coordinates and a number of the color for 
void breakoutDisplay_drawTile(int16_t xCoord, int16_t yCoord, uint16_t colorCode, bool erase)
{
    //if we are not erasing it
    if(!erase)
    {
        //draw that thing
        display_fillRect(xCoord, yCoord, TILE_WIDTH, TILE_HEIGHT, colorCode);
    }
    else//if we are
    {
        display_fillRect(xCoord, yCoord, TILE_WIDTH, TILE_HEIGHT, colorCode);
    }
}

// a fucntion that draws up the tiles for the start of the level.
void breakoutDisplay_drawNewTiles()
{
   
    int16_t currentXCoord = TILE_FIRST_X_COORD;
    int16_t currentYCoord = TILE_FIRST_Y_COORD;
    uint8_t colorNum = 0;
    uint16_t colorCode = breakoutDisplay_assignColorCode(colorNum);

    //this loop iterates through each row of tiles to be drawn
    for (int i = 0; i < COLOR_NUM_CYAN + 1; i++)
    {
        colorCode = breakoutDisplay_assignColorCode(colorNum);//set the current row color
        //draw the current row
        for (int j = 0; j < 9; j++)
        {
            breakoutDisplay_drawTile(currentXCoord, currentYCoord, colorCode, 0);
            currentXCoord += (TILE_WIDTH + TILE_SPACER_WIDTH);
        }
        currentXCoord = TILE_FIRST_X_COORD;
        currentYCoord += (TILE_HEIGHT + TILE_SPACER_WIDTH);
        colorNum++;
    }
}

// Convenience function that erases the entire tile area.
void breakoutDisplay_eraseAllTiles()
{
    //just fill the top half of the screen in black
    display_fillRect(0, 0, DISPLAY_WIDTH, (DISPLAY_HEIGHT / 2), DISPLAY_BLACK);
}

//this fucntion is to draw the ball that will be boucnign around the s
void breakoutDisplay_drawBall(int16_t xCoord, int16_t yCoord, bool erase)
{
    if (!erase)//of we arent erasing it draw that thang
    {
        display_fillCircle(xCoord, yCoord, BALL_WIDTH_HEIGHT, DISPLAY_WHITE);
    }
    else//we needa erase this thing now
    {
        display_fillCircle(xCoord, yCoord, BALL_WIDTH_HEIGHT, DISPLAY_BLACK);
    }
}