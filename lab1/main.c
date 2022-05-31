/*
This software is provided for student assignment use in the Department of
Electrical and Computer Engineering, Brigham Young University, Utah, USA.

Users agree to not re-host, or redistribute the software, in source or binary
form, to other persons or other institutions. Users may modify and use the
source code for personal or educational use.

For questions, contact Brad Hutchings or Jeff Goeders, https://ece.byu.edu/
*/

// Print out "hello world" on both the console and the LCD screen.

#include "display.h"

#define TEXT_SIZE 2
#define CURSOR_X 0
#define CURSOR_Y 0
#define DISPLAY_ORANGE 0xFBA0

#include <stdio.h>
int main() {
  display_init(); // Must init all of the software and underlying hardware for
                  // LCD.
  display_fillScreen(DISPLAY_BLACK);     // Blank the screen.
  display_setCursor(CURSOR_X, CURSOR_Y); // The upper left of the LCD screen.
  display_setTextColor(DISPLAY_RED);     // Make the text red.
  display_setTextSize(TEXT_SIZE);        // Make the text a little larger.
  // displays lines goig diagonally across the screen
  display_drawLine(DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, DISPLAY_DARK_GREEN);
  display_drawLine(0, DISPLAY_HEIGHT, DISPLAY_WIDTH, 0, DISPLAY_DARK_GREEN);
  // displays the two circles on the screen
  display_drawCircle(DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 2, 30, DISPLAY_ORANGE);
  display_fillCircle((DISPLAY_WIDTH / 4) * 3, DISPLAY_HEIGHT / 2, 30,
                     DISPLAY_ORANGE);
  // displays the two triangles
  display_fillTriangle(DISPLAY_WIDTH / 2, (DISPLAY_HEIGHT / 2) - 10,
                       (DISPLAY_WIDTH / 2) - 30, (DISPLAY_HEIGHT / 2) - 82,
                       (DISPLAY_WIDTH / 2) + 30, (DISPLAY_HEIGHT / 2) - 82,
                       DISPLAY_YELLOW);
  display_drawTriangle(DISPLAY_WIDTH / 2, (DISPLAY_HEIGHT / 2) + 10,
                       (DISPLAY_WIDTH / 2) - 30, (DISPLAY_HEIGHT / 2) + 82,
                       (DISPLAY_WIDTH / 2) + 30, (DISPLAY_HEIGHT / 2) + 82,
                       DISPLAY_YELLOW);
  printf("hello world!\n"); // This prints on the console.
  return 0;
}

// This function must be defined but can be left empty for now.
// You will use this function in a later lab.
// It is called in the timer interrupt service routine (see interrupts.c in
// supportFiles).
void isr_function() {
  // Empty for now.
}