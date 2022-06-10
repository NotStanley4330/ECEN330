/*
    File: touchHandler.h
    Author: Michael Norris
    Date: 6/9/2022
    Purpose: just deals with looking for touches from the screen
    as well as movement of the paddle left and right




*/ 

#include <stdbool.h>
#include <stdint.h>
#include "touchHandler.h"
#include "display.h"
#include "breakoutGlobals.h"
#include "breakoutDisplay.h"


// A number that does not represent a vlauid region for debugging
#define REGION_INVALID 12
// Since the tick happens every 100 ms and settle time for the ADC is 50 ms
// we only have to wait one cycle
#define ADC_SETTLED 2
//we can use this to set up a paddle movement speed in pixels per tick
#define PADDLE_MOVEMENT_SPEED 10 //this value is subject to change


//this flag will let us know if the touch handler has been enabled or not
static uint8_t touchHandlerEnabled = 0;
//this flag lets us know if the touchHandler has finished up its state stuff
static uint8_t touchHandlerCompleted = 0;
//this object should track the paddles properties for maintaining between states
static struct objectProperties paddle;
//this flag will let us know when the paddle has been released
static uint8_t screenIsReleased = 0;
//this timer will be incrememtned while the ADC is settling in
static uint8_t adcSettleTimer = 0;
// a variable to store the region returned
static uint8_t screenRegion = REGION_INVALID;




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
    //return the region value computed by the dispaly function
    return breakoutDisplay_computeRegionNumber(x, y);
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
    return screenIsReleased;
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
            if (touchHandlerEnabled)//wait for the machine to be enabled
            {
                //once enabled move to the awaiting touch state
                currentState = st_await_touch;
            }
            break;
        case st_await_touch:
            if (!touchHandlerEnabled)//if the machine has been disabled
            {
                currentState = st_init;//reset to the inital state
            }
            else//if we are still rocking and rolling
            {
                //wait for a touch
                if (display_isTouched())
                {
                    //clear old touch data out and move to waiting for the settling of the ADC
                    display_clearOldTouchData();
                    currentState = st_settle;
                }
            }
            break;
        case st_settle:
            //if we somehow got disabled in the 1 tick it takes for it to settle
            if (!touchHandlerEnabled)
            {
                //go back to the initial state
                currentState = st_init;
            }
            else//otherwise
            {
                if (adcSettleTimer == ADC_SETTLED)
                {
                    //reset our timer
                    adcSettleTimer = 0;
                    //retrieve the touched region
                    screenRegion = touchHandler_getRegionNumber();
                    //move to the next state
                    currentState = st_is_touched;
                    

                }
            }
            break;
        case st_is_touched:
            if (!touchHandlerEnabled)//if the machine got disable during touch (say we missed the ball)
            {
              currentState = st_init;//move to the initial state  
            }
            else//otherwise
            {
                //wait till the screen is released
                if (!display_isTouched())
                {
                    //move back to the awaiting touch state?
                    currentState = st_await_touch;
                }
            }

            break;
        case st_finished:
            
            break;
    }

    //switch case for state actions
    switch(currentState)
    {
        case st_init:
            //reset all flags and timers
            adcSettleTimer = 0;
            screenIsReleased = 0;
            touchHandlerCompleted = 0;
            screenRegion = REGION_INVALID;
            break;
        case st_await_touch:
            //DO NOTHING BUT WAIT AROUND
            break;
        case st_settle:
            //incrememnt the settle timer each tick while we wait
            adcSettleTimer++;
            break;
        case st_is_touched:
            //we will want to be trying to move this for as long as the screen is 
            //staying touched
            touchHandler_movePaddle(screenRegion, false);
            
            break;
        case st_finished:
            
            break;
    }
}


//this function handles the movement of the paddle from left to right taking the screen region
//into account and can even reset the paddle to its initial position if asked to 
void touchHandler_movePaddle(uint8_t screenRegion, bool reset)
{
    if (!reset)//if we have not been asked to reset the paddle
    {
        // decide what direction we need to move the paddle
        if (screenRegion == BREAKOUT_DISPLAY_REGION_0)//if left side is touched
        {
            if (paddle.xPosition == TILE_FIRST_X_COORD)//check if the paddle is hitting the side of the screen
            {
                //shouldnt be moving anything if this is true
            }
            //otherwise if the next movement will put us past our left edge
            else if (paddle.xPosition - PADDLE_MOVEMENT_SPEED <= TILE_FIRST_X_COORD)
            {
                //we want to erase and then put it stuck against the edge
                breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, true);
                paddle.xPosition = TILE_FIRST_X_COORD;//change the xposition to the edge
                //redraw it with the new position
                breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, false);
            }
            else//if it wont be hitting the edge
            {
                //erase the old paddle
                breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, true);
                //move the xpositions to the ekft by PADDLE_MOVEMENT_SPEED
                paddle.xPosition = paddle.xPosition - PADDLE_MOVEMENT_SPEED;
                //redraw the paddle in the new location
                breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, false);
            }
        }
        else if (screenRegion == BREAKOUT_DISPLAY_REGION_1)//if the right side is being touched
        {
            if (paddle.xPosition + PADDLE_WIDTH == DISPLAY_WIDTH - TILE_FIRST_X_COORD)//check if the paddle is against the right side fo the screen
            {
                //should be doing nothing if it is here
            }
            //if our movement will put the paddle beyond the edge of the screen
            else if (paddle.xPosition + PADDLE_MOVEMENT_SPEED + PADDLE_WIDTH >= DISPLAY_WIDTH - TILE_FIRST_X_COORD)
            {
                //erase the old paddle
                breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, true);
                //move the xPosition to be against the right edge
                paddle.xPosition = (DISPLAY_WIDTH - TILE_FIRST_X_COORD - PADDLE_WIDTH);
                //redraw the paddle in the new positon
                breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, false);
            }
            else//if our movement wont get us stuck
            {
                //erase the old paddle
                breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, true);
                //move it to the right by PADDLE_MOVEMENT_SPEED
                paddle.xPosition = paddle.xPosition + PADDLE_MOVEMENT_SPEED;
                //draw in the new paddle
                breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, false);
            }
        }
    }
    else//if we have been asked to reset the paddle
    {
        //erase the old paddle
        breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, true);
        //reset the coords back to the start
        paddle.xPosition = PADDLE_INIT_X_COORD;
        paddle.yPosition = PADDLE_INIT_Y_COORD;//this line should be totally unessary but its here if need be
        //draw in the new paddle
        breakoutDisplay_drawPaddle(paddle.xPosition, paddle.yPosition, false);
    }
}




struct objectProperties touchHandler_getPaddlePosition()
{
    //just return the paddle positon struct
    return paddle;
}