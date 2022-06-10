/*
    project: breakut for zybo board
    file: breakoutGlobals.h
    date: 6/8/2022
    purpose: this should contain some global things like struct definitions and color codes



*/

#include "display.h"
#include "xparameters.h"

//custom color defintions
#define DISPLAY_ORANGE 0xFBA0
#define DISPLAY_TEAL 0x0737
#define DISPLAY_PURPLE 0x8155

//the actual objects we have to deal with
struct objectProperties
{
    uint8_t xVelocity;
    uint8_t yVelocity;
    uint16_t xPosition;
    uint16_t yPosition;
};