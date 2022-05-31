/*
    File: verifySequence.c
    Author: Michael Norris
    Date: 5/31/2022
    Purpose: verifies that the correct qequence has been entered




*/


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "verifySequence.h"
#include "buttonHandler.h"
#include "simonDisplay.h"
#include "display.h"
#include "globals.h"
#include "buttons.h"

//a flag for enabling and disabling the verify sequence machine
static uint8_t verifySequenceEnabled = 0;
//a flag that shows if the sequence verification has finished
static uint8_t verifySequenceComplete = 0;
//a flag that goes high if the user hits an incorrect square
static uint8_t userWrongPress = 0;
//a flag that lets us know if the time for the user to press a button ran out
static uint8_t userTimedOut = 0;
//a timer for waiting till the user times out
static uint8_t userTimeOutTimer = 0;
//the current sequence position, should always start at 0
static uint8_t sequencePos = 0;

//the states for the verifySequence SM
enum  verifySequence_st_t
{
    st_init,                //inital state fo the machine
    st_enable_button,       //a state that enables the buttons to be pressed by the user
    st_wait_release,        //a state that waits for the buton to be let go
    st_verify_press,        //state to verify that the correct button press was made
    st_increment_seqeunce,  //state that icnrements the positon in the seq then returns to button enable
    st_finished             //state for when it is all finished
}

static enum verifySequence_st_t currentState = st_init;


// State machine will run when enabled.
void verifySequence_enable()
{
    //set the flag high
    verifySequenceEnabled = 1;
}

// This is part of the interlock. You disable the state-machine and then enable
// it again.
void verifySequence_disable()
{
    //set the flag low
    verifySequenceEnabled = 0;
}

// Standard init function.
void verifySequence_init()
{
    //set current state back to init just in case
    currentState = st_init;
}

// Used to detect if there has been a time-out error.
bool verifySequence_isTimeOutError()
{
    //retuns the appropriate flag
    return userTimedOut;
}

// Used to detect if the user tapped the incorrect sequence.
bool verifySequence_isUserInputError()
{
    //returns the flag for if the user messed up
    return userWrongPress;
}

// Used to detect if the verifySequence state machine has finished verifying.
bool verifySequence_isComplete()
{
    //return this flag
    return verifySequenceComplete;
}

// Standard tick function.
void verifySequence_tick()
{
    //switch statement for state transitions
    switch(currentState)
    {
        case st_init:
            //wait for the machine to be enabled
            if(verifySequenceEnabled)
            {
                //reset everything to zero
                sequencePos = 0;
                verifySequenceComplete = 0;
                userWrongPress = 0;
                userTimeOutTimer = 0;
                userTimedOut = 0;
                //move to the button waiting state
                currentState = st_enable_button;
            }
            break;
        case st_enable_button:
            //if we have been DISABLED
            if(!verifySequenceEnabled)
            {
                //go back to the beginning
                currentState = st_init
            }
            else//otherwise
            {
                //move to the waiting for release state after a single tick
                currentState = st_wait_release;
            }
            break;
        case st_wait_release:
            //if we have been DISABLED
            if(!verifySequenceEnabled)
            {
                //go back to the beginning
                currentState = st_init
            }
            break;
        case st_verify_press:

            break;
        case st_increment_seqeunce:

            break;
        case st_finished:

            break;
    }

    //switch statement for state actions
    switch(currentState)
    {
        case st_init:

            break;
        case st_enable_button:
            //enable the button handler sm
            buttonHandler_enable;
            break;
        case st_wait_release:

            break;
        case st_verify_press:

            break;
        case st_increment_seqeunce:

            break;
        case st_finished:

            break;
    }
}