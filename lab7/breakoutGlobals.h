/*
    project: breakut for zybo board
    file: breakoutGlobals.h
    date: 6/8/2022
    purpose: this should contain some global things like struct definitions and color codes



*/


#ifndef BREAKOUTGLOBALS_H_
#define BREAKOUTGLOBALS_H_

#include "display.h"
#include "xparameters.h"

//custom color defintions
#define DISPLAY_ORANGE 0xFBA0
#define DISPLAY_TEAL 0x0737
#define DISPLAY_PURPLE 0x8155


//the numebr of rows and columns of tiles we are working with
#define TILE_ROW_NUM 8
#define TILE_COLUMN_NUM 9

//the actual objects we have to deal with
struct objectProperties
{
    int8_t xVelocity;
    int8_t yVelocity;
    int16_t xPosition;
    int16_t yPosition;
};


struct tileItem
{
    int16_t xPosition;
    int16_t yPosition;
    uint8_t isDestroyed;
    uint16_t colorCode;
};

#endif//BREAKOUTGLOBALS_H_