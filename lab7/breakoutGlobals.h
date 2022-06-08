/*
    project: breakut for zybo board
    file: breakoutGlobals.h
    date: 6/8/2022
    purpose: this should contain some global things like struct definitions and color codes



*/

#include <display.h>
#include "xparameters.h"

//custom color defintions
#define DISPLAY_ORANGE 0xFBA0
#define DISPLAY_TEAL 0x0737
#define DISPLAY_PURPLE 0x8155

struct objectProperties
{
    uint8_t xVelocity;
    uint8_t yVelocity;
    uint8_t xPosition;
    uint8_t yPosition;
};