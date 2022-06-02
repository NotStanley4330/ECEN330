/*
    File: verifySequence.c
    Author: Michael Norris
    Date: 5/31/2022
    Purpose: verifies that the correct qequence has been entered




*/

#include "verifySequence.h"
#include "buttonHandler.h"
#include "buttons.h"
#include "display.h"
#include "globals.h"
#include "simonDisplay.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// if the user doesnt press the screen for 30 cycles (or three seconds) it
// should time out
#define TIME_OUT_CYCLES 30

// a flag for enabling and disabling the verify sequence machine
static uint8_t verifySequenceEnabled = 0;
// a flag that shows if the sequence verification has finished
static uint8_t verifySequenceComplete = 0;
// a flag that goes high if the user hits an incorrect square
static uint8_t userWrongPress = 0;
// a flag that lets us know if the time for the user to press a button ran out
static uint8_t userTimedOut = 0;
// a timer for waiting till the user times out
static uint8_t userTimeOutTimer = 0;
// the current sequence position, should always start at 0
static uint8_t sequencePos = 0;

// the states for the verifySequence SM
enum verifySequence_st_t {
  st_init,          // inital state fo the machine
  st_enable_button, // a state that enables the buttons to be pressed by the
                    // user
  st_wait_release,  // a state that waits for the buton to be let go
  st_verify_press,  // state to verify that the correct button press was made
  st_increment_seqeunce, // state that icnrements the positon in the seq then
                         // returns to button enable
  st_finished            // state for when it is all finished
};

static enum verifySequence_st_t currentState = st_init;

// State machine will run when enabled.
void verifySequence_enable() {
  // set the flag high
  verifySequenceEnabled = 1;
}

// This is part of the interlock. You disable the state-machine and then enable
// it again.
void verifySequence_disable() {
  // set the flag low
  verifySequenceEnabled = 0;
}

// Standard init function.
void verifySequence_init() {
  // set current state back to init just in case
  currentState = st_init;
}

// Used to detect if there has been a time-out error.
bool verifySequence_isTimeOutError() {
  // retuns the appropriate flag
  return userTimedOut;
}

// Used to detect if the user tapped the incorrect sequence.
bool verifySequence_isUserInputError() {
  // returns the flag for if the user messed up
  return userWrongPress;
}

// Used to detect if the verifySequence state machine has finished verifying.
bool verifySequence_isComplete() {
  // return this flag
  return verifySequenceComplete;
}

// Standard tick function.
void verifySequence_tick() {
  // switch statement for state transitions
  switch (currentState) {
  case st_init:
    // wait for the machine to be enabled
    if (verifySequenceEnabled) {
      // reset everything to zero
      sequencePos = 0;
      verifySequenceComplete = 0;
      userWrongPress = 0;
      userTimeOutTimer = 0;
      userTimedOut = 0;
      // move to the button waiting state
      currentState = st_enable_button;
    }
    break;
  case st_enable_button:
    // if we have been DISABLED
    if (!verifySequenceEnabled) {
      // go back to the beginning
      currentState = st_init;
    } else // otherwise
    {
      // move to the waiting for release state after a single tick
      currentState = st_wait_release;
    }
    break;
  case st_wait_release:
    // if we have been DISABLED
    if (!verifySequenceEnabled) {
      // go back to the beginning
      currentState = st_init;
    } else // if not
    {
      if (display_isTouched()) // do not want the timer to time out while the
                               // user is tocuhing it
      {
        // continually reset this till the screen is released
        userTimeOutTimer = 0;
      } else if (buttonHandler_releaseDetected()) // if the screen is untouched
                                                  // check if its been released
                                                  // or just never touched
      {
        // move to the state to verify the press against the sequence
        currentState = st_verify_press;
      } else if (userTimeOutTimer ==
                 TIME_OUT_CYCLES) // if a timeout happens while waiting for a
                                  // press
      {
        // raise the timeout flag
        userTimedOut = 1;
        // erase all the buttons
        simonDisplay_eraseAllButtons();
        // move to the done state
        currentState = st_finished;
      }
    }
    break;
  case st_verify_press:
    // if we have been DISABLED
    if (!verifySequenceEnabled) {
      // go back to the beginning
      currentState = st_init;
    } else // if not
    {
      currentState = st_increment_seqeunce;
    }
    break;
  case st_increment_seqeunce:
    // if we have been DISABLED
    if (!verifySequenceEnabled) {
      // go back to the beginning
      currentState = st_init;
    } else // if not
    {
      // first check if we have reached the end of the sequence or if the user
      // made an error
      if (sequencePos == (globals_getSequenceIterationLength()) ||
          userWrongPress) {
        // move to the finished state

        currentState = st_finished;
      } else // if not
      {
        // increment the sequence and restart the verification process
        sequencePos++;
        currentState = st_enable_button;
      }
    }
    break;
  case st_finished:
    if (!verifySequenceEnabled) // wait to be disabled
    {
      // once disabled, move to the start
      currentState = st_init;
    }

    break;
  }

  // switch statement for state actions
  switch (currentState) {
  case st_init:
    // if we have been disabled and moving back here we wann disable the button
    // handler as well
    buttonHandler_disable();
    break;
  case st_enable_button:
    // enable the button handler sm
    buttonHandler_enable();
    break;
  case st_wait_release:
    // increment this timer while waiting for the button to be released
    userTimeOutTimer++;
    break;
  case st_verify_press:
    // dsiable the buttonhandler machine now
    buttonHandler_disable();
    // check if the press matches the value in the sequence and if not
    if (globals_getSequenceValue(sequencePos) !=
        buttonHandler_getRegionNumber()) {
      // raise the error made flag
      userWrongPress = 1;
    }
    break;
  case st_increment_seqeunce:

    break;
  case st_finished:
    // raise the completed flag
    verifySequenceComplete = 1;
    break;
  }
}