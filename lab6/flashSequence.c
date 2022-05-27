/*
    File: simonDisplay.c
    Author: Michael Norris
    Date: 5/27/2022
    Purpose: flashes the sequence of buttons to be pressed




*/

#include "flashSequence.h"
#include "simonDisplay.h"
#include "display.h"
#include "utils.h"
#include "globals.h"
#include <stdint.h>
#include <stdio.h>

//we should wait for 10 ticks (or one second) before erasing the square and drawing the next one
#define FLASH_TIMER_WAIT 10


//this flag will be used to enable or disable the state machine
static uint8_t flashSequenceEnabled = 0;
//this counter will be used to wait between flashes in a sequence
static uint16_t betweenFlashesTimer = 0;
//this counter isnt timer based but is used to track which
//square in the sequnce is being drawn
static uint16_t sequncePos = 0;

// flash sequence state machine states
enum flashSequence_st_t {
    st_init,
    st_draw_sqaure,
    st_wait,
    st_erase_square,
    st_finished

};

 static enum flashSequence_st_t currentState = init_st;



// Turns on the state machine. Part of the interlock.
void flashSequence_enable()
{
    flashSequenceEnabled = 1;
}

// Turns off the state machine. Part of the interlock.
void flashSequence_disable()
{
    flashSequenceEnabled = 0;
}

// Standard init function.
void flashSequence_init();

// Other state machines can call this to determine if this state machine is
// finished.
bool flashSequence_isComplete();

// Standard tick function.
void flashSequence_tick();