/*
File: clockDisplay.c
Author: Michael Norris
Date created: May 14 2022
Description:
    handles the displaying of the actual clock and incrementing and decrmenting
the numbers along with properly handling rollover. Includes a test fucntion to
demonstrate clock function



*/

// include the necessary header files
#include "clockDisplay.h"
#include "display.h"
#include "utils.h"
// the screenDefines.h file holds all screen related #define values for
// convenience
//#include "screenDefines.h"
// include stdio for text output and formatting
#include <stdio.h>

// all of the screen defintions follow here
// this value can be modified to change the size
// of the text on the display, this program supports
// sizes from 3 to 6 (default is 6)
#define SCREENDEFINES_CLOCK_TEXT_SIZE 3

// the centers of the scren both vertical and horizontal
#define SCREENDEFINES_SCREEN_CENTER_VERT (DISPLAY_HEIGHT / 2)
#define SCREENDEFINES_SCREEN_CENTER_HORIZ (DISPLAY_WIDTH / 2)
// the vetical sections of the screen for touch sectors
#define SCREENDEFINES_HOURS_SECTION 106
#define SCREENDEFINES_SECONDS_SECTION 213

// colon character sperating each value from each other
#define SCREENDEFINES_CLOCK_SEPERATOR ":"

// x positions on the screen where each of the numerical values should be
// written
#define SCREENDEFINES_HOURS_COUNTER_CURSOR                                     \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ - (23 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_MINUTES_COUNTER_CURSOR                                   \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ - (5 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_SECONDS_COUNTER_CURSOR                                   \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ + (13 * SCREENDEFINES_CLOCK_TEXT_SIZE))
// x positions for the colons between the numbers
#define SCREENDEFINES_COLON_HOURS_MINUTES_CURSOR                               \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ - (11 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_COLON_MINUTES_SECONDS_CURSOR                             \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ + (7 * SCREENDEFINES_CLOCK_TEXT_SIZE))
// y position for the whole clock to draw on
#define SCREENDEFINES_CLOCK_CURSOR_Y_POSITION                                  \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (3.5 * SCREENDEFINES_CLOCK_TEXT_SIZE))

// positional values for the triangle over the minutes number
#define SCREENDEFINES_TRIANGLE_UP_MINS_X1 SCREENDEFINES_SCREEN_CENTER_HORIZ
#define SCREENDEFINES_TRIANGLE_UP_MINS_Y1                                      \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (15 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_MINS_X2                                      \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ - (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_MINS_Y2                                      \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_MINS_X3                                      \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ + (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_MINS_Y3                                      \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))

// positional values for the triangle over the seconds number
#define SCREENDEFINES_TRIANGLE_UP_SECS_X1                                      \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ + (18 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_SECS_Y1                                      \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (15 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_SECS_X2                                      \
  (SCREENDEFINES_TRIANGLE_UP_MINS_X2 + (18 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_SECS_Y2                                      \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_SECS_X3                                      \
  (SCREENDEFINES_TRIANGLE_UP_SECS_X2 + (11 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_SECS_Y3                                      \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))

// positional values for the triangle over the hours number
#define SCREENDEFINES_TRIANGLE_UP_HOURS_X1                                     \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ - (17 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_HOURS_Y1                                     \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (15 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_HOURS_X2                                     \
  (SCREENDEFINES_TRIANGLE_UP_MINS_X2 - (16 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_HOURS_Y2                                     \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_HOURS_X3                                     \
  (SCREENDEFINES_TRIANGLE_UP_HOURS_X2 + (11 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_UP_HOURS_Y3                                     \
  (SCREENDEFINES_SCREEN_CENTER_VERT - (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))

// positonal values for the triangle under the minutes number
#define SCREENDEFINES_TRIANGLE_DOWN_MINS_X1 SCREENDEFINES_SCREEN_CENTER_HORIZ
#define SCREENDEFINES_TRIANGLE_DOWN_MINS_Y1                                    \
  (SCREENDEFINES_SCREEN_CENTER_VERT + (15 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_MINS_X2                                    \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ - (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_MINS_Y2                                    \
  (SCREENDEFINES_SCREEN_CENTER_VERT + (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_MINS_X3                                    \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ + (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_MINS_Y3                                    \
  (SCREENDEFINES_SCREEN_CENTER_VERT + (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))

// positional values for the triangle under the seconds number
#define SCREENDEFINES_TRIANGLE_DOWN_SECS_X1                                    \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ + (18 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_SECS_Y1                                    \
  (SCREENDEFINES_SCREEN_CENTER_VERT + (15 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_SECS_X2                                    \
  (SCREENDEFINES_TRIANGLE_UP_MINS_X2 + (18 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_SECS_Y2                                    \
  (SCREENDEFINES_SCREEN_CENTER_VERT + (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_SECS_X3                                    \
  (SCREENDEFINES_TRIANGLE_UP_SECS_X2 + (11 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_SECS_Y3                                    \
  (SCREENDEFINES_SCREEN_CENTER_VERT + (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))

// positional values for the triangle under the hours number
#define SCREENDEFINES_TRIANGLE_DOWN_HOURS_X1                                   \
  (SCREENDEFINES_SCREEN_CENTER_HORIZ - (17 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_HOURS_Y1                                   \
  (SCREENDEFINES_SCREEN_CENTER_VERT + (15 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_HOURS_X2                                   \
  (SCREENDEFINES_TRIANGLE_UP_MINS_X2 - (16 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_HOURS_Y2                                   \
  (SCREENDEFINES_SCREEN_CENTER_VERT + (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_HOURS_X3                                   \
  (SCREENDEFINES_TRIANGLE_UP_HOURS_X2 + (11 * SCREENDEFINES_CLOCK_TEXT_SIZE))
#define SCREENDEFINES_TRIANGLE_DOWN_HOURS_Y3                                   \
  (SCREENDEFINES_SCREEN_CENTER_VERT + (6 * SCREENDEFINES_CLOCK_TEXT_SIZE))

// string formatter for the hours so that it will be a decimal with no leading
// 0s
#define HOURS_DISPLAY_FORMAT "%2hd"
// string formatter for seconds and minutes so that they will display as a
// decimal with a leading 0
#define MINS_SECS_DISPLAY_FORMAT "%02hd"
// max and min values for the hours as it can only display values between 1 & 12
// (never 0)
#define HOURS_MAX_VALUE 12
#define HOURS_MIN_VALUE 1
// max and rollunder values for minutes and seconds as it should only dispaly
// valeus 0 to 59 (never 60)
#define MINS_SECS_ROLL_UNDER                                                   \
  0 // this can double as a min value as well because we will us uints
#define MINS_SECS_MAX_VALUE 59

// varibles to store current and previous time values in
uint8_t currentHoursValue = 1; // initialize as one since thats the min value
uint8_t currentMinsValue = 0;
uint8_t currentSecsValue = 0;

uint8_t prevHoursValue = 1; // same as above, 1 is min value
uint8_t prevMinsValue = 0;
uint8_t prevSecsValue = 0;

// strings to store the values converted to formatted strings (only need to be 2
// chars long)
char hoursString[16];
char minsString[16];
char secsString[16];

// Called only once - performs any necessary inits.
// This is a good place to draw the triangles and any other
// parts of the clock display that will never change.
void clockDisplay_init() {
  // initialize the display so we can use it
  display_init();
  // fill display with black
  display_fillScreen(DISPLAY_BLACK);
  // set the text size as defined in screenDefines.h
  display_setTextSize(SCREENDEFINES_CLOCK_TEXT_SIZE);
  // set the text color to be green with black background
  display_setTextColor(DISPLAY_GREEN);
  display_setTextColorBg(DISPLAY_GREEN, DISPLAY_BLACK);

  // draw the colons and then the triangles as these will never be
  // replaced/redrawn while the program runs

  // set cursor to the first colon position and draw it
  display_setCursor(SCREENDEFINES_COLON_HOURS_MINUTES_CURSOR,
                    SCREENDEFINES_CLOCK_CURSOR_Y_POSITION);
  display_println(SCREENDEFINES_CLOCK_SEPERATOR);
  // set cursor to second colon position and draw it
  display_setCursor(SCREENDEFINES_COLON_MINUTES_SECONDS_CURSOR,
                    SCREENDEFINES_CLOCK_CURSOR_Y_POSITION);
  display_println(SCREENDEFINES_CLOCK_SEPERATOR);
  // DRAW ALL THE TRONGLES IN GREEN
  display_fillTriangle(
      SCREENDEFINES_TRIANGLE_UP_HOURS_X1, SCREENDEFINES_TRIANGLE_UP_HOURS_Y1,
      SCREENDEFINES_TRIANGLE_UP_HOURS_X2, SCREENDEFINES_TRIANGLE_UP_HOURS_Y2,
      SCREENDEFINES_TRIANGLE_UP_HOURS_X3, SCREENDEFINES_TRIANGLE_UP_HOURS_Y3,
      DISPLAY_GREEN);
  display_fillTriangle(SCREENDEFINES_TRIANGLE_DOWN_HOURS_X1,
                       SCREENDEFINES_TRIANGLE_DOWN_HOURS_Y1,
                       SCREENDEFINES_TRIANGLE_DOWN_HOURS_X2,
                       SCREENDEFINES_TRIANGLE_DOWN_HOURS_Y2,
                       SCREENDEFINES_TRIANGLE_DOWN_HOURS_X3,
                       SCREENDEFINES_TRIANGLE_DOWN_HOURS_Y3, DISPLAY_GREEN);
  display_fillTriangle(
      SCREENDEFINES_TRIANGLE_UP_MINS_X1, SCREENDEFINES_TRIANGLE_UP_MINS_Y1,
      SCREENDEFINES_TRIANGLE_UP_MINS_X2, SCREENDEFINES_TRIANGLE_UP_MINS_Y2,
      SCREENDEFINES_TRIANGLE_UP_MINS_X3, SCREENDEFINES_TRIANGLE_UP_MINS_Y3,
      DISPLAY_GREEN);
  display_fillTriangle(
      SCREENDEFINES_TRIANGLE_DOWN_MINS_X1, SCREENDEFINES_TRIANGLE_DOWN_MINS_Y1,
      SCREENDEFINES_TRIANGLE_DOWN_MINS_X2, SCREENDEFINES_TRIANGLE_DOWN_MINS_Y2,
      SCREENDEFINES_TRIANGLE_DOWN_MINS_X3, SCREENDEFINES_TRIANGLE_DOWN_MINS_Y3,
      DISPLAY_GREEN);
  display_fillTriangle(
      SCREENDEFINES_TRIANGLE_UP_SECS_X1, SCREENDEFINES_TRIANGLE_UP_SECS_Y1,
      SCREENDEFINES_TRIANGLE_UP_SECS_X2, SCREENDEFINES_TRIANGLE_UP_SECS_Y2,
      SCREENDEFINES_TRIANGLE_UP_SECS_X3, SCREENDEFINES_TRIANGLE_UP_SECS_Y3,
      DISPLAY_GREEN);
  display_fillTriangle(
      SCREENDEFINES_TRIANGLE_DOWN_SECS_X1, SCREENDEFINES_TRIANGLE_DOWN_SECS_Y1,
      SCREENDEFINES_TRIANGLE_DOWN_SECS_X2, SCREENDEFINES_TRIANGLE_DOWN_SECS_Y2,
      SCREENDEFINES_TRIANGLE_DOWN_SECS_X3, SCREENDEFINES_TRIANGLE_DOWN_SECS_Y3,
      DISPLAY_GREEN);

  // force the clock display to update to the default values
  clockDisplay_updateTimeDisplay(1);
}

// Updates the time display with latest time, making sure to update only those
// digits that have changed since the last update. if forceUpdateAll is true,
// update all digits.
void clockDisplay_updateTimeDisplay(bool forceUpdateAll) {
  // if the hours value has changed or it is being forced to update
  if (currentHoursValue != prevHoursValue || forceUpdateAll) {
    // update the previous Hours value to match the current one
    prevHoursValue = currentHoursValue;
    // convert the value into a formatted string
    sprintf(hoursString, HOURS_DISPLAY_FORMAT, currentHoursValue);
    // set the cursor to the correct spot and draw the hours value
    display_setCursor(SCREENDEFINES_HOURS_COUNTER_CURSOR,
                      SCREENDEFINES_CLOCK_CURSOR_Y_POSITION);
    display_println(hoursString);
  }
  // if the minutes value has changed or it is being forced to update
  if (currentMinsValue != prevMinsValue || forceUpdateAll) {
    // update the previous Hours value to match the current one
    prevMinsValue = currentMinsValue;
    // convert the value into a formatted string
    sprintf(minsString, MINS_SECS_DISPLAY_FORMAT, currentMinsValue);
    // set the cursor to the correct spot and draw the hours value
    display_setCursor(SCREENDEFINES_MINUTES_COUNTER_CURSOR,
                      SCREENDEFINES_CLOCK_CURSOR_Y_POSITION);
    display_println(minsString);
  }
  // if the minutes value has changed or it is being forced to update
  if (currentSecsValue != prevSecsValue || forceUpdateAll) {
    // update the previous Hours value to match the current one
    prevSecsValue = currentSecsValue;
    // convert the value into a formatted string
    sprintf(secsString, MINS_SECS_DISPLAY_FORMAT, currentSecsValue);
    // set the cursor to the correct spot and draw the hours value
    display_setCursor(SCREENDEFINES_SECONDS_COUNTER_CURSOR,
                      SCREENDEFINES_CLOCK_CURSOR_Y_POSITION);
    display_println(secsString);
  }
}

// increment the hours value and handle roll over
void clockDisplay_hoursInc() {
  // if the value was already 12 set to 0
  if (currentHoursValue == HOURS_MAX_VALUE) {
    // set the hours val to 1
    currentHoursValue = HOURS_MIN_VALUE;
  } else // if not add one
  {
    currentHoursValue++;
  }
}

// decrement the hours value and handle roll under
void clockDisplay_hoursDec() {
  // if the value was already zero roll back to 59
  if (currentHoursValue == HOURS_MIN_VALUE) {
    // set the hours val to 12
    currentHoursValue = HOURS_MAX_VALUE;
  } else // if not already one just subtract one
  {
    currentHoursValue--;
  }
}

// increment the minutes value and handle roll over
void clockDisplay_minutesInc() {
  // if the value was already 59 set to 0
  if (currentMinsValue == MINS_SECS_MAX_VALUE) {
    // set the mins val to 0
    currentMinsValue = MINS_SECS_ROLL_UNDER;
  } else // if not already zero just add one
  {
    currentMinsValue++;
  }
}

// decrement the minutes value and handle roll under
void clockDisplay_minutesDec() {
  // if the value was already zero roll back to 59
  if (currentMinsValue == MINS_SECS_ROLL_UNDER) {
    // set the mins val to 59
    currentMinsValue = MINS_SECS_MAX_VALUE;
  } else // if not already zero just subtract one
  {
    currentMinsValue--;
  }
}

// increment the secs value and handle roll over
void clockDisplay_secondsInc() {
  // if the value was already max set to zero
  if (currentSecsValue == MINS_SECS_MAX_VALUE) {
    // set the secs val to 0
    currentSecsValue = MINS_SECS_ROLL_UNDER;
  } else // if not already zero just add one
  {
    currentSecsValue++;
  }
}

// decrement the minutes value and handle roll under
void clockDisplay_secondsDec() {
  // if the value was already zero roll back to 59
  if (currentSecsValue == MINS_SECS_ROLL_UNDER) {
    // set the secs val to 59
    currentSecsValue = MINS_SECS_MAX_VALUE;
  } else // if not already zero just subtract one
  {
    currentSecsValue--;
  }
}

// Reads the touched coordinates and performs the increment or decrement,
// depending upon the touched region.
void clockDisplay_performIncDec() {
  // set up three varaibles for the coords and pressure (z) on the touch screen
  uint16_t x = 0;
  uint16_t y = 0;
  uint8_t z = 0;
  // get the point of the screen that was touched

  display_getTouchedPoint(&x, &y, &z);

  // check if ti was in the top half of the screen
  if (y < SCREENDEFINES_SCREEN_CENTER_VERT) {
    // if the touch is in the hour inc section
    if (x < SCREENDEFINES_HOURS_SECTION) {
      // increment hours
      clockDisplay_hoursInc();
    } else if (x < SCREENDEFINES_SECONDS_SECTION) // if in the minutes section
    {
      // increment minutes
      clockDisplay_minutesInc();
    } else // if its in the seconds section
    {
      // inc seconds
      clockDisplay_secondsInc();
    }
  } else if (y > SCREENDEFINES_SCREEN_CENTER_VERT) {
    // if the touch is in the hour inc section
    if (x < SCREENDEFINES_HOURS_SECTION) {
      // decrement hours
      clockDisplay_hoursDec();
    } else if (x < SCREENDEFINES_SECONDS_SECTION) // if in the minutes section
    {
      // decrement minutes
      clockDisplay_minutesDec();
    } else // if its in the seconds section
    {
      // dec seconds
      clockDisplay_secondsDec();
    }
  }

  // update the clock to the new values
  clockDisplay_updateTimeDisplay(0);
}

// Advances the time forward by 1 second and update the display.
void clockDisplay_advanceTimeOneSecond() {
  // first add one to seconds
  currentSecsValue++;

  // if the seconds rolls over reset to zero and increment the minutes
  if (currentSecsValue > MINS_SECS_MAX_VALUE) {
    currentSecsValue = 0;
    currentMinsValue++;
  }
  // if minutes rolls over reset to zero and add one to the hours
  if (currentMinsValue > MINS_SECS_MAX_VALUE) {
    currentMinsValue = 0;
    currentHoursValue++;
  }
  // if hours rolls over reset to 1
  if (currentHoursValue > HOURS_MAX_VALUE) {
    currentHoursValue = HOURS_MIN_VALUE;
  }

  // update the display with the new values
  clockDisplay_updateTimeDisplay(0);
}

// Run a test of clock-display functions.
void clockDisplay_runTest() {
  // initialize the clock
  clockDisplay_init();
  // force update the clock again to be sure
  clockDisplay_updateTimeDisplay(1);

  // 10 times increment the hours
  for (int i = 0; i < 10; i++) {
    // call hours increment fucntiom
    clockDisplay_hoursInc();
    // update the display
    clockDisplay_updateTimeDisplay(0);
    // wait a second
    utils_msDelay(1000);
  }

  // 10 times decrement the hours
  for (int i = 0; i < 10; i++) {
    // call hours decrement fucntiom
    clockDisplay_hoursDec();
    // update the display
    clockDisplay_updateTimeDisplay(0);
    // wait a second
    utils_msDelay(1000);
  }

  // 10 times increment the mins
  for (int i = 0; i < 10; i++) {
    // call hours increment fucntiom
    clockDisplay_minutesInc();
    // update the display
    clockDisplay_updateTimeDisplay(0);
    // wait a second
    utils_msDelay(1000);
  }

  // 10 times decrement the mins
  for (int i = 0; i < 10; i++) {
    // call hours decrement fucntiom
    clockDisplay_minutesDec();
    // update the display
    clockDisplay_updateTimeDisplay(0);
    // wait a second
    utils_msDelay(1000);
  }

  // 10 times increment the secs
  for (int i = 0; i < 10; i++) {
    // call hours increment fucntiom
    clockDisplay_secondsInc();
    // update the display
    clockDisplay_updateTimeDisplay(0);
    // wait a second
    utils_msDelay(1000);
  }

  // 10 times decrement the secs
  for (int i = 0; i < 10; i++) {
    // call hours decrement fucntiom
    clockDisplay_secondsDec();
    // update the display
    clockDisplay_updateTimeDisplay(0);
    // wait a second
    utils_msDelay(1000);
  }

  // run the clokc at 10x speed for 10 seconds (100 simulated seconds)
  for (int i = 0; i < 100; i++) {
    // advance the clock one second
    clockDisplay_advanceTimeOneSecond();
    // update the clock display
    clockDisplay_updateTimeDisplay(0);
    // wait a tenth of a second before advancing again;
    utils_msDelay(100);
  }
}