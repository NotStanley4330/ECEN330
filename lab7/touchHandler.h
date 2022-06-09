/*
    File: touchHandler.h
    Author: Michael Norris
    Date: 6/6/2022
    Purpose: just deals with looking for touches from the screen




*/ 

#ifndef TOUCHHANDLER_H_
#define TOUCHHANDLER_H_

#include <stdbool.h>
#include <stdint.h>


//this just gets us the half of the screen that has been touched
uint8_t touchHandler_getRegionNumber();

// Turn on the state machine.
void touchHandler_enable();

// Turn off the state machine.
void touchHandler_disable();

// Standard init function.
void touchHandler_init();

//this returns the boolean value of a flag set by the state machine
//this flag is set when a dispaly is untouched
bool touchHandler_releaseDetected();

// Returns wether or not the screen has been touched
bool touchHandler_isComplete();

// Standard tick function.
void touchHandler_tick();

//this function handles the movement of the paddle from left to right taking the screen region
//into account and can even reset the paddle to its initial position if asked to 
void touchHandler_movePaddle(uint8_t screenRegion, bool reset);


#endif //TOUCHHANDLER_H_