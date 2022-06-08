/*
    File: touchHandler.h
    Author: Michael Norris
    Date: 6/6/2022
    Purpose: just deals with looking for touches from the screen




*/ 

#include <stdbool.h>
#include <stdint.h>
#include "touchHandler.h"
#include "display.h"
#include "breakoutGlobals.h"
#include "breakoutDisplay.h"


//this flag will let us know if the touch handler has been enabled or not
static uint8_t touchHandlerEnabled = 0;
//this flag lets us know if the touchHandler has finished up its state stuff
static uint8_t touchHandlerCompleted = 0;
//this object should track the paddles properties for maintaining between states
static struct objectProperties paddle;
//this flag will let us know when the paddle has been released
static uint8_t screenUnTouched = 0;


enum touchHandler_st_t {
  st_init,        // the initial state of the machine
  st_await_touch, // state for awaiting the touch of a user
  st_settle,      // a state for the machine to wait in while the ADC settles
  st_is_touched,  // a state that moves the paddle left or right while touched (until it hits the side of the screen)
  st_untouched,   // a state that stops the movement of the paddle
  st_finished     // ending state of the machine

};

static enum touchHandler_st_t currentState;//the actual state register for the touchhandler

//this just gets us the half of the screen that has been touched
uint8_t touchHandler_getRegionNumber()
{
    // varaibles for coords and pressure on the screen
    int16_t x, y;
    uint8_t z;
    // get the touch values from the ADC
    display_getTouchedPoint(&x, &y, &z);

}

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
void touchHandler_init()
{
    //set the intial values for the paddle and the sm to the initial state
    paddle.xVelocity = 0;
    paddle.yVelocity = 0;
    paddle.xPosition = PADDLE_INIT_X_COORD;
    paddle.yPosition = PADDLE_INIT_Y_COORD;
    currentState = st_init;

}

//this returns the boolean value of a flag set by the state machine
//this flag is set when a dispaly is untouched
bool touchHandler_releaseDetected()
{
    //just return the value of the screen untouched flag
    return screenUnTouched;
}

// Returns wether or not the screen has been touched
bool touchHandler_isComplete()
{
    return touchHandlerCompleted;
}

// Standard tick function.
void touchHandler_tick()
{
    //switch case for state transitions
    switch(currentState)
    {
        case st_init:

            break;
        case st_await_touch:

            break;
        case st_settle:

            break;
        case st_is_touched:

            break;
        case st_untouched:

            break;
        case st_finished:
            
            break;
    }

    //switch case for state actions
    switch(currentState)
    {
        case st_init:

            break;
        case st_await_touch:

            break;
        case st_settle:

            break;
        case st_is_touched:

            break;
        case st_untouched:

            break;
        case st_finished:
            
            break;
    }
}