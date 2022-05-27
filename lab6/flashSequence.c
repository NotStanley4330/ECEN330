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

//this flag will be used to enable or disable the state machine
static uint8_t flastSequenceEnabled = 0;


// Turns on the state machine. Part of the interlock.
void flashSequence_enable();

// Turns off the state machine. Part of the interlock.
void flashSequence_disable();

// Standard init function.
void flashSequence_init();

// Other state machines can call this to determine if this state machine is
// finished.
bool flashSequence_isComplete();

// Standard tick function.
void flashSequence_tick();