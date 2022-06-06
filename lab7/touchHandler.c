/*
    File: touchHandler.h
    Author: Michael Norris
    Date: 6/6/2022
    Purpose: just deals with looking for touches from the screen




*/ 

#include <stdbool.h>
#include <stdint.h>
#include "touchHandler.h"

//this flag will let us know if the touch handler has been enabled or not
static uint8_t touchHandlerEnabled = 0;
//this flag lets us know if the touchHandler has finished up its state stuff
static uint8_t tocuhHandlerCompleted = 0;



//this just gets us the half of the screen that has been touched
uint8_t touchHandler_getRegionNumber();

// Turn on the state machine.
void touchHandler_enable()
{
    //jsut enable that thang
    touchHandlerEnabled = 1;
}

// Turn off the state machine.
void touchHandler_disable()
{
    //IVE BEEN DISABLEDDD
    touchHandlerEnabled = 0;
}

// Standard init function.
void touchHandler_init();

//this returns the boolean value of a flag set by the state machine
//this flag is set when a dispaly is untouched
bool touchHandler_releaseDetected();

// Returns wether or not the screen has been touched
bool touchHandler_isComplete()
{
    return tocuhHandlerCompleted;
}

// Standard tick function.
void touchHandler_tick();