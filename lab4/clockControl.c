/*
File: clockControl.c
Author: Michael Norris
Date created: May 17 2022
Description:




*/

#include "clockControl.h"
#include "clockDisplay.h"
#include "display.h"
#include <stdio.h>

// All printed messages for states are provided here.
#define INIT_ST_MSG "init state\n"
#define NEVER_TOUCHED_ST_MSG "never_touched_st\n"
#define WAITING_FOR_TOUCH_ST_MSG "waiting for touch_st\n"
#define AD_TIMER_RUNNING_ST_MSG "ad_timer_running_st\n"
#define AUTO_TIMER_RUNNING_ST_MSG "auto_timer_running_st\n"
#define RATE_TIMER_RUNNING_ST_MSG "rate_timer_running_st\n"
#define RATE_TIMER_EXPIRED_ST_MSG "rate_timer_expired_st\n"

// our period is ~100ms so as not to miss interrupts but also for ease of use
// when waiting a half second. Because of this we end up using just one tick for
// the ADC to settles thus ADC_SETTLE_TIME is about 100 ms
#define ADC_SETTLE_TIME 1
// because of our 100 ms period we only need 5 ticks for the auto timer to go up
#define HOLD_TIMER_FINISH 5
// the next value is the rate at which we want the auto timer to icnrement/
// decrement since we want it to go x10 of normal time it should change ever 100
// ms or 1 period
#define AUTO_INC_DEC_FINISHED 1
// when the timer runs normally, we jsut want to add one second per second, thus
// we should wait 1000ms or 10 periods before updating
#define SECOND_WAIT_FINISHED 10

// this is a timer we increment while waiting for ADC to settle
static uint16_t adcTimer = 0;
// this is a timer we incrememt for the about 0.5 seconds for the button to stay
// held down before we begin to auto increment it
static uint16_t holdTimer = 0;
// a counter we increment to wait out for 1 second before adding a second to the
// clock
static uint16_t secondWaitTimer = 0;
// a counter that incrememtns at the rate of 10 seconds per second for the auto
// increment and decrement phases
static uint32_t autoIncDecTimer = 0;

// States for the controller state machine.
enum clockControl_st_t {
  init_st, // Start here, transition out of this state on the first tick.
  never_touched_st, // Wait here until the first touch - clock is disabled until
                    // set.
  waiting_for_touch_st,  // waiting for touch, clock is enabled and running.
  ad_timer_running_st,   // waiting for the touch-controller ADC to settle.
  auto_timer_running_st, // waiting for the auto-update delay to expire
                         // (user is holding down button for auto-inc/dec)
  rate_timer_running_st, // waiting for the rate-timer to expire to know when to
                         // perform the auto inc/dec.
  rate_timer_expired_st, // when the rate-timer expires, perform the inc/dec
                         // function.
  add_second_to_clock_st // add a second to the clock time and reset the ms
                         // counter.
};
static enum clockControl_st_t currentState;

// This is a debug state print routine. It will print the names of the states
// each time tick() is called. It only prints states if they are different than
// the previous state.
void debugStatePrint() {
  static enum clockControl_st_t previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state
  // name over and over.
  if (previousState != currentState || firstPass) {
    firstPass = false; // previousState will be defined, firstPass is false.
    previousState =
        currentState;     // keep track of the last state that you were in.
    switch (currentState) // This prints messages based upon the state that you
                          // were in.
    {
    case init_st:
      printf(INIT_ST_MSG);
      break;
    case never_touched_st:
      printf(NEVER_TOUCHED_ST_MSG);
      break;
    case waiting_for_touch_st:
      printf(WAITING_FOR_TOUCH_ST_MSG);
      break;
    case ad_timer_running_st:
      printf(AD_TIMER_RUNNING_ST_MSG);
      break;
    case auto_timer_running_st:
      printf(AUTO_TIMER_RUNNING_ST_MSG);
      break;
    case rate_timer_running_st:
      printf(RATE_TIMER_RUNNING_ST_MSG);
      break;
    case rate_timer_expired_st:
      printf(RATE_TIMER_EXPIRED_ST_MSG);
      break;
    }
  }
}

// Standard tick function.
void clockControl_tick() {
  // Perform state update first.
  switch (currentState) {
  case init_st:
    // move from the initial state to the never touched state on first tick
    currentState = never_touched_st;
    break;
  case never_touched_st:
    // wait for the display to be touched
    if (display_isTouched()) {
      // clear out the old touch data
      display_clearOldTouchData();
      // advance state to the ad_timer_running_st
      currentState = ad_timer_running_st;
    }
    break;
  case waiting_for_touch_st:
    // wait for the display to be touched
    if (display_isTouched()) {
      // clear out the old touch data
      display_clearOldTouchData();
      // advance state to the ad_timer_running_st
      currentState = ad_timer_running_st;
    }
    break;
  case ad_timer_running_st:
    // if the display is still touched and the timer has finished go to the auto
    // increment timer wait
    if (display_isTouched() && adcTimer == ADC_SETTLE_TIME) {
      // advance to the auto inc timer state
      currentState = auto_timer_running_st;
    }
    // if the adc has settled and it is no longer touched
    else if (!display_isTouched() && ADC_SETTLE_TIME) {
      // perform an increment or decrement depending on where touched
      clockDisplay_performIncDec();
      // go to the waiting for touch state
      currentState = waiting_for_touch_st;
    }
    break;
  case auto_timer_running_st:
    // if the screen is still being touched after half a second
    if (display_isTouched() && holdTimer == HOLD_TIMER_FINISH) {
      // move to the rate timer state
      currentState = rate_timer_running_st;
    }
    // if the screen has stopped being touched during this time
    else if (!display_isTouched()) {
      // perform an increment or decrement depending on where touched
      clockDisplay_performIncDec();
      // go to the waiting for touch state
      currentState = waiting_for_touch_st;
    }
    break;
  case rate_timer_running_st:
    // we want to go and update the clock once ever 100 ms
    // so if the screen is still touched when timer runs out
    if (display_isTouched() && autoIncDecTimer == AUTO_INC_DEC_FINISHED) {
      currentState = rate_timer_expired_st;
    }
    // if the display is no longer being touched
    else if (!display_isTouched()) {
      // go back to waiting for a touch
      currentState = waiting_for_touch_st;
    }
    break;
  case rate_timer_expired_st:
    // check to make sure the dispaly is still touched
    if (display_isTouched()) {
      // inc or dec based on location
      clockDisplay_performIncDec();
      // go back to the timer state to wait another 100ms
      currentState = rate_timer_running_st;
    }
    // if screen is no longer touched
    else if (!display_isTouched()) {
      // go back to wait
      currentState = waiting_for_touch_st;
    }
    break;
  default:
    // print an error message here.
    break;
  }

  // Perform state action next.
  switch (currentState) {
  case init_st: // this state does nothing and only serves as the intiial state
                // before transition
    break;
  case never_touched_st:
    // this state also basically does nothing except wait for a touch
    break;
  case waiting_for_touch_st:
    // reset all internal timers to 0
    adcTimer = 0;
    holdTimer = 0;
    autoIncDecTimer = 0;
    // we need to wait a second before adding another second to the timer
    if (secondWaitTimer == SECOND_WAIT_FINISHED) // if we have waited one second
    {
      // add a second to the clock
      clockDisplay_advanceTimeOneSecond();
      // reset the second counter
      secondWaitTimer = 0;
    } else {
      secondWaitTimer++;
    }
    break;
  case ad_timer_running_st:
    // increment the adc timer so we can wait for it to settle
    adcTimer++;
    break;
  case auto_timer_running_st:
    // increment the holdTimer so we can wait 0.5 seconds to auto increment
    holdTimer++;
    break;
  case rate_timer_running_st:
    // we want this timer to run so we can change the value ten times a second
    autoIncDecTimer++;
    break;
  case rate_timer_expired_st:
    // reset the timer here so we can run it again
    autoIncDecTimer = 0;
    break;
  default:
    // print an error message here.
    break;
  }
}

// Call this before you call clockControl_tick().
void clockControl_init() {
  // set the current state to the initial state
  currentState = init_st;
}