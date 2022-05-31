/*
File: screenDefines.h
Author: Michael Norris
Date created: May 14 2022
Description:
This file contains all of the #defines for positions of things on the screen
for convenience and clarity sake. It should properly resize for clock text sizes
between 3 and 6


*/
#include "display.h"

#ifndef SCREENDEFINES_H_
#define SCREENDEFINES_H_

// this value can be modified to change the size
// of the text on the display, this program supports
// sizes from 3 to 6 (default is 6)
#define SCREENDEFINES_CLOCK_TEXT_SIZE 6

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

#endif