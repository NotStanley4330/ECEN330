/*
    File: buttonHandler.c
    Author: Michael Norris
    Date: 5/24/2022
    Purpose:




*/


#include "buttonHandler.h"
#include "simonDisplay.h"
#include "utils.h"
#include "display.h"
#include <stdio.h>

//DEFINES

// Since the tick happens every 100 ms and settle time for the ADC is 50 ms
//we only have to wait one cycle
#define ADC_SETTLED 1
// A number that does not represent a vlauid region for debugging
#define REGION_INVALID 12

// a value that lets the SM know if it is enabled or not
static uint8_t handlerEnable = 0;
//a quick 1 cycle timer to increment for the ADC to setlte
static uint8_t adcSettleTimer = 0;
//a value that lets us know whether the buttons have already been drawn 
//to avoid flashing the screen to redraw
//static uint8_t buttonsHaveBeenDrawn = 0;
//a value to track when the touch screen has been released from being touched
static uint8_t touchScreenIsReleased = 0;
// a variable to store the region returned
static uint8_t screenRegion = REGION_INVALID;

// The buttonHandler SM states defined
enum buttonHandler_st_t {
    st_init,            //the initial state of the machine
    st_await_touch,     //state for awaiting the touch of a user
    st_settle,          //a state for the machine to wait in while the ADC settles
    st_is_touched,      //a state that changes the size of the touched square and waits
    st_untouched,       //a state for reverting the screen back to nomrla after the touch is released
    st_finished         //ending state of the machine

}; 

static enum buttonHandler_st_t currentState;//the state register itself







//FUNCTIONS DEFINED BY THE .h FILE



//gets the touched point on the screen when called then returns a 0, 1, 2, or 3 from the simonDisplay_computeRegionNumber()
//function representing the region touched
uint8_t buttonHandler_getRegionNumber()
{
    //varaibles for coords and pressure on the screen
    int16_t x, y;
    uint8_t z;
    //get the touch values from the ADC
    display_getTouchedPoint(&x, &y, &z);
    //returns a 0, 1, 2, or 3 representing the touched region
    return simonDisplay_computeRegionNumber(x, y);

}

// Turn on the state machine. Part of the interlock.
void buttonHandler_enable()
{
    //set the enabled value high
    handlerEnable = 1;
}

// Turn off the state machine. Part of the interlock.
void buttonHandler_disable()
{
    //reset the enabled value to 0 to disable the machine
    handlerEnable;
    //clear the touch screen release flag
    touchScreenIsReleased = 0;
    //reset the machine to its intial state
    currentState = st_init;

}

// Standard init function.
void buttonHandler_init()
{
    currentState = st_init;
}

// The only thing this function does is return a boolean flag set by the
// buttonHandler state machine. To wit: Once enabled, the buttonHandler
// state-machine first waits for a touch. Once a touch is detected, the
// buttonHandler state-machine computes the region-number for the touched area.
// Next, the buttonHandler state-machine waits until the player removes their
// finger. At this point, the state-machine should set a bool flag that
// indicates the the player has removed their finger. Once the buttonHandler()
// state-machine is disabled, it should clear this flag.
// All buttonHandler_releasedDetected() does is return the value of this flag.
// As such, the body of this function should only contain a single line of code.
// If this function does more than return a boolean set by the buttonHandler
// state machine, you are going about this incorrectly.
bool buttonHandler_releaseDetected()
{
    //return the value of touchScreenISreleased
    return touchScreenIsReleased;
}

// Let's you know that the buttonHander is waiting in the interlock state.
bool buttonHandler_isComplete();

// Standard tick function.
void buttonHandler_tick()
{
    //first perform state transitions then state actions

    //state transitions
    switch(currentState)
    {
        case st_init:
            //if the SM has been enabled move to the waiting state
            if (handlerEnable)
            {
                currentState = st_await_touch;//move to the await touch state
            }
            break;
        case st_await_touch:
            //if the machine has been disabled, go back to the initial state
            if (!handlerEnable)
            {
                currentState = st_init;
            }
            else//if not keep waiting for a touch
            {
                //wait for a tocuch
                if(display_isTouched())
                {
                    //clear out old touch data and wait for
                    //ADC settling
                    display_clearOldTouchData();
                    currentState = st_settle;

                }
            }
            break;
        case st_settle:
            //if SM disabled during this state return to the initial state
            if (!handlerEnable)
            {
                currentState = st_init;
            }
            else//if not disabled 
            {
                if (adcSettleTimer == ADC_SETTLED)//if the ADC has had time to settle
                {
                    //reset the timer
                    adcSettleTimer = 0;
                    //get the tocuhed region
                    screenRegion = buttonHandler_getRegionNumber();
                    //enlarge the square where the screen was touched
                    simonDisplay_drawSquare(screenRegion, 0);
                    //move to the is_tocuhed state
                    currentState = st_is_touched;
                }
            }
            break;
        case st_is_touched:
            //if SM disabled during this state return to the initial state
            if (!handlerEnable)
            {
                currentState = st_init;
            }
            else//if not disabled
            {
               //wait until the screen is no longer touched
               if(!display_isTouched())
               {
                   //move to the untouched state
                   currentState = st_untouched;
               }

            }
            break;
        case st_untouched:
        //move to finished state
         //if SM disabled during this state return to the initial state
            if (!handlerEnable)
            {
                currentState = st_init;
            }
            else//if not disabled
            {
                currentState = st_finished;
            }
            break;
        case st_finished:
            if(!handlerEnable)//wait for the machine to be disable to go back to the initial state
            {
                currentState = st_init;
            }

            break;
    }

    //state actions
    switch(currentState)
    {
        case st_init:
            //set or reset all state variables to intial values
            adcSettleTimer = 0;
            touchScreenIsReleased = 0;
            screenRegion = REGION_INVALID;
            break;
        case st_await_touch:
            break;
        case st_settle:
            adcSettleTimer++; //incrememnt the sttel timer each tick this state is still current
            break;
        case st_is_touched:
            break;
        case st_untouched:
            // erase the enlarged square
            simonDisplay_drawSquare(screenRegion, 1);
            // redraw the button for correct region
            simonDisplay_drawButton(screenRegion, 0);
            //set the flag for isReleased
            touchScreenIsReleased = 1;

            break;
        case st_finished:
            break;
    }




}

// Allows an external controller to notify the buttonHandler that a time-out has
// occurred.
void buttonHandler_timeOutOccurred()
{

}