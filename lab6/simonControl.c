/*
    File: simonControl.c
    Author: Michael Norris
    Date: 6/2/2022
    Purpose: essentially runs the simon game in its entirety




*/

// just include basically everything
#include "simonControl.h"
#include "buttonHandler.h"
#include "buttons.h"
#include "display.h"
#include "flashSequence.h"
#include "globals.h"
#include "simonDisplay.h"
#include "utils.h"
#include "verifySequence.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// my defines for the text

// text for message shown at game over
#define MSG_GAME_END "Longest Sequence"
// text that prompts the user to tocuh for the next level of seq
#define MSG_NEXT_LEVEL "Touch for next level"
// Text for the title screen
#define MSG_TITLE "SIMON"
// subtext for the title screen touch prompt
#define MSG_TOUCH "TOUCH TO START"
// text for a succesfully completed sequence
#define MSG_SUCCESS "YAY!"
// title text size
#define TXT_SIZE_TITLE 5
// size for all other texts
#define TXT_SIZE_GENERAL 2
// how long to display "yay" for until moving on (3 seconds)
#define SUCCESS_WAIT_TIME 30
// coords for the title text
#define TITLE_COORD_X 80
#define TITLE_COORD_Y 90
// coords for "tocuh to start"
#define TOUCH_COORD_X 70
#define TOUCH_COORD_Y 140
// coords for success message
#define SUCCESS_COORD_X 120
#define SUCCESS_COORD_Y 100
// coord of the game over text
#define GAME_END_COORD_X 50
// coord for the highest level value
#define GAME_END_LEVEL_COORD_X 250

// always start with a sequence of 4
#define SEQ_START_LEN 4
// we want to genearte a sequence with only values 0-3, thus we need
// to do a % 4 operation
#define SEQ_GENERATE_DIVISOR 4

// always wanna start with a length of 4
static uint8_t currentLength = SEQ_START_LEN;
// value of where we are at in the current sequence, will start at one
// and go to the sequence length before going to 1 again
static uint8_t currentSequencePos = 1;
// an array that stores the current simon sequence
static uint8_t simonSequence[GLOBALS_MAX_FLASH_SEQUENCE] = {0};
// flag to say if/when the user has failed
static uint8_t userError = 0;
// counter that increments until we want the yay screen to be done and move on
static uint8_t successScreenTimer = 0;
// a counter to increment for the end screen to wait around
static uint8_t endScreenTimer = 0;
// a counter i need to add a description for
static uint8_t newScreenTimer = 0;
// a flag that says wehter the screen has been drawn and intialized
// and simon is ready to go
static uint8_t simonStarted = 0;

// our seed value for srand.
// We can set this to the amuont of cycles it took for the user to start a new
// game
static uint8_t randSeed = 0;
// a flag to say wether we should draw the title scren again
//(as we dont wanna do this if they are contuining on or flash it)
static uint8_t titleDrawn = 0;
// flag to say if we should generate a new sequence
static uint8_t newSequenceGenerate = 0;
// a flag for enabling or disabling the state machine
static uint8_t simonControlEnabled = 0;

// a string for the message for the ending level
static char lastLevelReachedString[10] = " ";

// the states for the simonControl SM
enum simonControl_st_t {
  st_init,
  st_wait_for_release,
  st_game_start,
  st_flash_enable,
  st_flash_wait,
  st_flash_disable,
  st_verify_enable,
  st_verify_wait,
  st_verfiy_disable,
  st_end_game_print,
  st_end_game_wait,
  st_success_print,
  st_success_wait,
  st_newlevel_print,
  st_newlevel_wait
};

static enum simonControl_st_t currentState =
    st_init; // always should start with init

// a custom function to generate pseudorandom sequences
void simonControl_generateSimonSequence() {
  // seed the random number machine
  srand(randSeed);
  // iterate through the sequence assigning values between 0 & 3
  for (int i = 0; i < currentLength; i++) {
    simonSequence[i] = rand() % (SEQ_GENERATE_DIVISOR);
  }
}

// Used to init the state machine. Always provided though it may not be
// necessary.
void simonControl_init() { currentState = st_init; }

// Standard tick function.
void simonControl_tick() {
  // swtich statement for state transitions
  switch (currentState) {
  case st_init:
    if (simonControlEnabled) // wait for the machine to be enabled
    {

      if (newSequenceGenerate) // if we need to generate a new seq we are gonna
                               // do that then go into a new game
      {
        simonControl_generateSimonSequence(); // generate our seqeunce
        // set the global sequence to our generated one
        globals_setSequence(simonSequence, currentLength);
        // move to the game start phase
        // draw in the buttons here to avoid flashing
        simonDisplay_drawAllButtons();
        currentState = st_game_start;
      }
      // if that isnt true we are sitting on the title screen and
      // waiting for user input
      else if (display_isTouched()) // if we have gotten a user touch
      {
        // gen a sequence and set it globally
        simonControl_generateSimonSequence();
        globals_setSequence(simonSequence, currentLength);
        // move to state to wait for screen release
        currentState = st_wait_for_release;
      }
    }
    break;
  case st_wait_for_release:
    // we need to wait for the screen to be release before blanking the screen
    // and drawing the buttons
    if (!display_isTouched()) {
      // display_fillScreen(DISPLAY_BLACK);
      // using display balcking cause timer interrupt misses so i need it to be
      // quicker clear the simon title and tocuh to start text
      display_setTextColor(DISPLAY_BLACK);
      display_setCursor(TITLE_COORD_X, TITLE_COORD_Y);
      display_setTextSize(TXT_SIZE_TITLE);
      display_println(MSG_TITLE);
      display_setCursor(TOUCH_COORD_X, TOUCH_COORD_Y);
      display_setTextSize(TXT_SIZE_GENERAL);
      display_println(MSG_TOUCH);
      display_setTextColor(DISPLAY_WHITE);
      // draw in the buttons here to avoid flashing
      simonDisplay_drawAllButtons();

      currentState = st_game_start;
    }
    break;
  case st_game_start:
    // since all we do here is set sequence pos and move on just
    // move to the next state
    // set the sequence global iterantion length
    globals_setSequenceIterationLength(currentSequencePos);
    currentState = st_flash_enable;
    break;
  case st_flash_enable:
    // move to the state where we wait for the flashing to be done
    currentState = st_flash_wait;

    break;
  case st_flash_wait:
    // wait for the flashing to be done before moving on
    if (flashSequence_isComplete()) {
      // move to the disabler state
      currentState = st_flash_disable;
    }
    break;
  case st_flash_disable:
    // now that its been disabled move to the verifier
    currentState = st_verify_enable;
    break;
  case st_verify_enable:
    // now its enabled move to the waiter
    currentState = st_verify_wait;
    break;
  case st_verify_wait:
    // we only want to move on if verification is complete
    if (verifySequence_isComplete()) {
      currentState = st_verfiy_disable;
    }
    break;
  case st_verfiy_disable:
    // now that we have disbaled it
    // check if we have gottne through the sequence
    if (currentSequencePos == currentLength) {
      // print a cuccess message
      currentState = st_success_print;
    }
    // if the player lsot because they miss-pressed or tiemd out
    else if (verifySequence_isTimeOutError() ||
             verifySequence_isUserInputError()) {
      userError = 1;
      currentState = st_end_game_print;
    }
    // if the player has not finished nor lost, move to the next level of
    // the current sequence
    else {
      // inc and move back to the start of the flash and verify sequence
      currentSequencePos++;
      currentState = st_game_start;
    }
    break;
  case st_success_print:
    // once the message has printed we can move to he watiing state
    currentState = st_success_wait;
    break;
  case st_success_wait:
    // we need to wait around for a certain amount of time before clearing the
    // message
    if (successScreenTimer == SUCCESS_WAIT_TIME) {
      // reset the timer
      successScreenTimer = 0;
      // clear the screen out
      // display_fillScreen(DISPLAY_BLACK);
      // blacking the screen takes enough time you can miss interrupts so we
      // need to make it more efficient we will just wipe the text and not the
      // whole screen
      display_setTextColor(DISPLAY_BLACK);
      display_setCursor(SUCCESS_COORD_X, SUCCESS_COORD_Y);
      display_setTextSize(TXT_SIZE_GENERAL);
      display_println(MSG_SUCCESS);
      display_setTextColor(DISPLAY_WHITE);
      // move to the print new level state
      currentState = st_newlevel_print;
    }
    break;

  case st_newlevel_print:
    // after printing move right on to the waiting game
    currentState = st_newlevel_wait;
    break;
  case st_newlevel_wait:
    // wait for if the display was touched
    if (display_isTouched()) {
      // clear out the next level text
      display_setTextColor(DISPLAY_BLACK);
      display_setTextSize(TXT_SIZE_GENERAL);
      display_setCursor(GAME_END_COORD_X, TOUCH_COORD_Y);
      display_println(MSG_NEXT_LEVEL);
      display_setTextColor(DISPLAY_WHITE);
      // icnrement the level of the sequence
      currentLength++;
      // rest seqeunce position to one
      currentSequencePos = 1;
      // rest all the flags but the title screen one
      successScreenTimer = 0;
      newScreenTimer = 0;
      endScreenTimer = 0;
      // set the flag true that we are going to a new sequence
      newSequenceGenerate = 1;
      currentState = st_init;
    } else if (newScreenTimer ==
               SUCCESS_WAIT_TIME) // if the user hasnt pressed the screen for 3
                                  // seconds they dont want to continue
    {
      // clear out the next level text
      display_setTextColor(DISPLAY_BLACK);
      display_setTextSize(TXT_SIZE_GENERAL);
      display_setCursor(GAME_END_COORD_X, TOUCH_COORD_Y);
      display_println(MSG_NEXT_LEVEL);
      display_setTextColor(DISPLAY_WHITE);
      currentState = st_end_game_print;
    }

    break;
  case st_end_game_print:
    // after printing play the waiting game
    currentState = st_end_game_wait;
    break;
  case st_end_game_wait:
    // wait a bit for the endscreen before blanking it
    if (endScreenTimer == SUCCESS_WAIT_TIME) {
      // erase the text only to speed things up
      display_setTextColor(DISPLAY_BLACK);
      display_setCursor(GAME_END_COORD_X, TOUCH_COORD_Y);
      display_setTextSize(TXT_SIZE_GENERAL);
      display_println(MSG_GAME_END);
      display_setCursor(GAME_END_LEVEL_COORD_X, TOUCH_COORD_Y);
      display_println(lastLevelReachedString);
      display_setTextColor(DISPLAY_WHITE);

      // reset the current length//display the simon title and tocuh to start
      // text
      display_setCursor(TITLE_COORD_X, TITLE_COORD_Y);
      display_setTextSize(TXT_SIZE_TITLE);
      display_println(MSG_TITLE);
      display_setCursor(TOUCH_COORD_X, TOUCH_COORD_Y);
      display_setTextSize(TXT_SIZE_GENERAL);
      display_println(MSG_TOUCH);
      endScreenTimer = 0;
      newScreenTimer = 0;
      titleDrawn = 0;
      simonStarted = 0;
      newSequenceGenerate = 0;
      userError = 0;
      currentSequencePos = 1;
      currentLength = SEQ_START_LEN;
      currentState = st_init;
    }
    break;
  }
  // switch statement for state actions
  switch (currentState) {
  case st_init:
    if (!simonStarted) // if the game has not yet started, get it rolling
    {
      // if the title has not been drawn do it
      if (!titleDrawn) {
        // display the simon title and tocuh to start text
        display_setCursor(TITLE_COORD_X, TITLE_COORD_Y);
        display_setTextSize(TXT_SIZE_TITLE);
        display_println(MSG_TITLE);
        display_setCursor(TOUCH_COORD_X, TOUCH_COORD_Y);
        display_setTextSize(TXT_SIZE_GENERAL);
        display_println(MSG_TOUCH);
        // set the titleDrawn flag high so that we dont flash this screen
        titleDrawn = 1;
      }
      // set the started flag high so we can move to the next state
      simonStarted = 1;
    }
    // have all the flags reset just in case
    userError = 0;

    randSeed++; // we will increment the seed while just waiting here on the
                // title screen
    break;
  case st_wait_for_release:
    // dont need to do anything here but wait around
    break;
  case st_game_start:
    // set the current sequene iteration
    globals_getSequenceIterationLength(currentSequencePos);
    // draw the buttons

    break;
  case st_flash_enable:
    // enable the sequence flashing machine
    flashSequence_enable();
    break;
  case st_flash_wait:
    // do nothing but wait around
    break;
  case st_flash_disable:
    // turn off the flasher
    flashSequence_disable();
    break;
  case st_verify_enable:
    // just enable the verifier
    verifySequence_enable();
    break;
  case st_verify_wait:
    // do nothing but wait around
    break;
  case st_verfiy_disable:
    // make the verify state machine DISABLED
    verifySequence_disable();
    break;
  case st_success_print:
    // erase all the buttons
    simonDisplay_eraseAllButtons();
    // move the cursor to the right spot and print the yay message
    display_setCursor(SUCCESS_COORD_X, SUCCESS_COORD_Y);
    display_setTextSize(TXT_SIZE_GENERAL);
    display_println(MSG_SUCCESS);
    break;
  case st_success_wait:
    // icnrement this timer while waiting
    successScreenTimer++;
    break;

  case st_newlevel_print:
    // print out the next level text
    display_setTextSize(TXT_SIZE_GENERAL);
    display_setCursor(GAME_END_COORD_X, TOUCH_COORD_Y);
    display_println(MSG_NEXT_LEVEL);
    break;
  case st_newlevel_wait:
    // increment this timer so we can time out if they dont touch for new level
    newScreenTimer++;
    break;
  case st_end_game_print:
    // erase all the buttons
    simonDisplay_eraseAllButtons();
    // set up the text in the right spot and print the message and level
    display_setCursor(GAME_END_COORD_X, TOUCH_COORD_Y);
    display_setTextSize(TXT_SIZE_GENERAL);
    display_setTextColor(DISPLAY_WHITE);
    display_println(MSG_GAME_END);
    display_setCursor(GAME_END_LEVEL_COORD_X, TOUCH_COORD_Y);
    // format the string for the latest level reached
    sprintf(lastLevelReachedString, "%i/%i", currentSequencePos, currentLength);
    display_println(lastLevelReachedString);
    break;
  case st_end_game_wait:
    endScreenTimer++;
    break;
  }
}

// Enables the control state machine.
void simonControl_enable() {
  // set the enable flag
  simonControlEnabled = 1;
}

// Disables the control state machine.
void simonControl_disable() {
  // unset the enable flag
  simonControlEnabled = 0;
}

// If you want to stop the game after a mistake, check this function after each
// tick.
bool simonControl_isGameOver() { return userError; }

// Use this to set the sequence length. This the sequence set by this function
// will only be recognized when the controller passes through its init state.
void simonControl_setSequenceLength(uint16_t length);