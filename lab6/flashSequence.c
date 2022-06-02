/*
    File: flashSequence.c
    Author: Michael Norris
    Date: 5/27/2022
    Purpose: flashes the sequence of buttons to be pressed




*/

#include "flashSequence.h"
#include "display.h"
#include "globals.h"
#include "simonDisplay.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>

// we should wait for 10 ticks (or one second) before erasing the square and
// drawing the next one
#define FLASH_TIMER_WAIT 10

// this flag will be used to enable or disable the state machine
static uint8_t flashSequenceEnabled = 0;
// this value retruns true if the sequence has been completed but 0 otherwise
static uint8_t flashSequenceCompleted = 0;
// this counter will be used to wait between flashes in a sequence
static uint16_t betweenFlashesTimer = 0;
// this counter isnt timer based but is used to track which
// square in the sequnce is being drawn
static uint16_t sequencePos = 0;

// flash sequence state machine states
enum flashSequence_st_t {
  st_init,
  st_draw_sqaure,
  st_wait,
  st_erase_square,
  st_finished

};

static enum flashSequence_st_t currentState = st_init;

// Turns on the state machine. Part of the interlock.
void flashSequence_enable() { flashSequenceEnabled = 1; }

// Turns off the state machine. Part of the interlock.
void flashSequence_disable() { flashSequenceEnabled = 0; }

// Standard init function.
void flashSequence_init() { currentState = st_init; }

// Other state machines can call this to determine if this state machine is
// finished.
bool flashSequence_isComplete() { return flashSequenceCompleted; }

// Standard tick function.
void flashSequence_tick() {
  // switch statement for SM transitions
  switch (currentState) {
  case st_init:
    // wait for the SM to be enabled before starting the sequence
    if (flashSequenceEnabled) {
      currentState = st_draw_sqaure;
      // draw all the squares just in case only the first time
      simonDisplay_drawAllButtons();
    }
    break;
  case st_wait:
    // if the SM has been disabled
    if (!flashSequenceEnabled) {
      // retrun to initail state
      currentState = st_init;
      // erase the last drawn square
      simonDisplay_drawSquare(globals_getSequenceValue(sequencePos), 1);
      // draw back the appropriate button in the sequence
      simonDisplay_drawButton(globals_getSequenceValue(sequencePos), 0);

    } else // if we are still waiting
    {
      // if the timer has run for at least a second
      if (betweenFlashesTimer >= FLASH_TIMER_WAIT) {
        // reset the counter and move to the erasing state
        currentState = st_erase_square;
      }
    }
    break;
  case st_draw_sqaure:
    // if the flash equence has been disabled go back to the beginning
    if (!flashSequenceEnabled) {
      // move back to init state
      currentState = st_init;
      // erase the last drawn square
      simonDisplay_drawSquare(globals_getSequenceValue(sequencePos), 1);
      // draw back the appropriate button in the sequence
      simonDisplay_drawButton(globals_getSequenceValue(sequencePos), 0);
    } else // if still enabled
    {
      // move to the wait to erase state
      currentState = st_wait;
    }
    break;
  case st_erase_square:
    // check again if we have been disbaled by acid
    if (!flashSequenceEnabled) {
      // move back to the init state
      currentState = st_init;
    } else // if we are still no disabled
    {
      // if we have gotten through all values in the sequence
      if (sequencePos >= globals_getSequenceIterationLength()) {
        // we are finished and move to that state
        currentState = st_finished;
      } else // otherwise we should continue with the sequence
      {
        sequencePos++;
        currentState = st_draw_sqaure;
      }
    }
    break;
  case st_finished:
    // once we are finally disabled
    if (!flashSequenceEnabled) {
      currentState = st_init;
      // reset all the flags and timer just in case
      sequencePos = 0;
      betweenFlashesTimer = 0;
      flashSequenceCompleted = 0;
    }
    break;
  }

  // switch statement for SM actions
  switch (currentState) {
  case st_init:
    sequencePos = 0;
    betweenFlashesTimer = 0;
    flashSequenceCompleted = 0;
    break;
  case st_wait:
    // increment the counter to wait to flash the next square
    betweenFlashesTimer++;
    break;
  case st_draw_sqaure:
    // draw the square appropriate for the place in the sequence
    simonDisplay_drawSquare(globals_getSequenceValue(sequencePos), 0);
    // reset the wait timer
    betweenFlashesTimer = 0;
    break;
  case st_erase_square:
    // erase the square and redraw the button
    simonDisplay_drawSquare(globals_getSequenceValue(sequencePos), 1);
    simonDisplay_drawButton(globals_getSequenceValue(sequencePos), 0);
    // reset the wait timer
    betweenFlashesTimer = 0;
    break;
  case st_finished:
    flashSequenceCompleted = 1;
    break;
  }
}