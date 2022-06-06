/*
    Author: Michael Norris
    Date: May 3, 2022
    File: buttons.c
    Desc: Takes the input from the four buttons and uses tha to write
    rectangles to the display



*/

#include "buttons.h"
#include "display.h"
#include "xil_io.h"
#include "xparameters.h"

// Input is 1111
#define BUTTONS_GPIO_DIRECTION_IS_INPUT 0xF
// Tri state offset is BASE_ADDRESS + 4
#define BUTTONS_TRI_STATE_OFFSET 0X04
// GPIO base address
#define BUTTONS_GPIO_DEVICE_BASE_ADDRESS XPAR_PUSH_BUTTONS_BASEADDR

// define the size of text on screen
#define BUTTONS_TEXT_SIZE 2

// Bitmask checking if all buttons are pressed
#define BUTTONS_ALL_PRESSED 0xF

// Each buttons rectangle will be the same size so same values
#define BUTTONS_RECT_LENGTH 80
#define BUTTONS_RECT_HEIGHT 120

// All button's text is on the same line so same y coord
#define BUTTONS_TEXT_Y_CORD 55

// Button 0's screen rectangle values
#define BUTTONS_BTN0_X_CORD 239
#define BUTTONS_BTN0_TEXT_X_CORD 256
#define BUTTONS_BTN0_TEXT "BTN0"

// Button 1's screen rectangle values
#define BUTTONS_BTN1_X_CORD 159
#define BUTTONS_BTN1_TEXT_X_CORD 176
#define BUTTONS_BTN1_TEXT "BTN1"

// Button 2's screen rectangle values
#define BUTTONS_BTN2_X_CORD 79
#define BUTTONS_BTN2_TEXT_X_CORD 96
#define BUTTONS_BTN2_TEXT "BTN2"

// Button 3's screen rectangle values
#define BUTTONS_BTN3_X_CORD 0
#define BUTTONS_BTN3_TEXT_X_CORD 16
#define BUTTONS_BTN3_TEXT "BTN3"

// helper function that reads in from the GPIO Button register
int32_t buttons_readGpioRegister(int32_t offset) {
  // returns the value of the address using the low level Xilinx call
  return Xil_In32(BUTTONS_GPIO_DEVICE_BASE_ADDRESS + offset);
}

// helper function to write a value to the specified address with offset
void buttons_writeGpioRegister(int32_t offset, int32_t value) {
  // Xilinx call that writes the value to the address
  Xil_Out32(BUTTONS_GPIO_DEVICE_BASE_ADDRESS + offset, value);
}

// Initializes the button driver software and hardware. Returns one of the
// defined status values (above).
int32_t buttons_init() {
  // get the current value of the tri state register
  uint32_t triStateValue = buttons_readGpioRegister(BUTTONS_TRI_STATE_OFFSET);
  // set up the GPIO register to be an input by setting the low 4 bits to be 1
  // and then writing it back
  triStateValue = triStateValue | BUTTONS_GPIO_DIRECTION_IS_INPUT;
  buttons_writeGpioRegister(BUTTONS_TRI_STATE_OFFSET, triStateValue);
  // return the ok signal
  return BUTTONS_INIT_STATUS_OK;
}

// Returns the current value of all 4 buttons as the lower 4 bits of the
// returned value. bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
int32_t buttons_read() {
  // read in the values of the 4 buttons and return it
  uint32_t buttonsInputValues = buttons_readGpioRegister(0);
  return buttonsInputValues;
}

// Runs a test of the buttons. As you push the buttons, graphics and messages
// will be written to the LCD panel. The test will until all 4 pushbuttons are
// simultaneously pressed.
void buttons_runTest() {
  buttons_init();
  // initialize the display to be written to
  display_init();
  // fill the dsiplay with black
  display_fillScreen(DISPLAY_BLACK);
  // set up variables for current and previous input values of the buttons
  // both of these values will update on each while loop
  uint32_t currVal = 0;
  uint32_t prevVal = 0;
  // set the display text size to a value of 2
  display_setTextSize(BUTTONS_TEXT_SIZE);

  // loop until all 4 buttons are simultaneously pressed
  while (currVal ^ BUTTONS_ALL_PRESSED) {
    // copy the current value into prevVal
    prevVal = currVal;
    // read in the current button inputs
    currVal = buttons_read();
    // IF square for BTN0 is already drawn dont draw it again
    if (!(prevVal & BUTTONS_BTN0_MASK) && (currVal & BUTTONS_BTN0_MASK)) {
      // fill the far right rectangle yellow
      display_fillRect(BUTTONS_BTN0_X_CORD, 0, BUTTONS_RECT_LENGTH,
                       BUTTONS_RECT_HEIGHT, DISPLAY_YELLOW);
      // set text cursor to the center of the rectangle
      display_setCursor(BUTTONS_BTN0_TEXT_X_CORD, BUTTONS_TEXT_Y_CORD);
      // make the text color black
      display_setTextColor(DISPLAY_BLACK);
      // write 'BTNO' to the screen
      display_println(BUTTONS_BTN0_TEXT);
    }
    // if the square was drawn and the button is no longer pressed
    else if ((prevVal & BUTTONS_BTN0_MASK) && !(currVal & BUTTONS_BTN0_MASK)) {
      // fill the square back in with black
      display_fillRect(BUTTONS_BTN0_X_CORD, 0, BUTTONS_RECT_LENGTH,
                       BUTTONS_RECT_HEIGHT, DISPLAY_BLACK);
    }

    // IF square for BTN1 is already drawn dont draw it again
    if (!(prevVal & BUTTONS_BTN1_MASK) && (currVal & BUTTONS_BTN1_MASK)) {
      // fil the second from the right rectangle with green
      display_fillRect(BUTTONS_BTN1_X_CORD, 0, BUTTONS_RECT_LENGTH,
                       BUTTONS_RECT_HEIGHT, DISPLAY_GREEN);
      // set text cursor to the center of the rectangle
      display_setCursor(BUTTONS_BTN1_TEXT_X_CORD, BUTTONS_TEXT_Y_CORD);
      // set text color black
      display_setTextColor(DISPLAY_BLACK);
      // write 'BTN1' to the screen
      display_println(BUTTONS_BTN1_TEXT);
    }
    // if the square was drawn and now button is not pressed
    else if ((prevVal & BUTTONS_BTN1_MASK) && !(currVal & BUTTONS_BTN1_MASK)) {
      // fill the square back in with black
      display_fillRect(BUTTONS_BTN1_X_CORD, 0, BUTTONS_RECT_LENGTH,
                       BUTTONS_RECT_HEIGHT, DISPLAY_BLACK);
    }

    // IF square for BTN2 is already drawn dont draw it again
    if (!(prevVal & BUTTONS_BTN2_MASK) && (currVal & BUTTONS_BTN2_MASK)) {
      // fil the second from the right rectangle with green
      display_fillRect(BUTTONS_BTN2_X_CORD, 0, BUTTONS_RECT_LENGTH,
                       BUTTONS_RECT_HEIGHT, DISPLAY_RED);
      // set text cursor to the center of the rectangle
      display_setCursor(BUTTONS_BTN2_TEXT_X_CORD, BUTTONS_TEXT_Y_CORD);
      // set text color black
      display_setTextColor(DISPLAY_BLACK);
      // write 'BTN2' to the screen
      display_println(BUTTONS_BTN2_TEXT);
    }
    // if the square was drawn and now button is not pressed
    else if ((prevVal & BUTTONS_BTN2_MASK) && !(currVal & BUTTONS_BTN2_MASK)) {
      // fill the square back in with black
      display_fillRect(BUTTONS_BTN2_X_CORD, 0, BUTTONS_RECT_LENGTH,
                       BUTTONS_RECT_HEIGHT, DISPLAY_BLACK);
    }

    // IF square for BTN2 is already drawn dont draw it again
    if (!(prevVal & BUTTONS_BTN3_MASK) && (currVal & BUTTONS_BTN3_MASK)) {
      // fil the second from the right rectangle with green
      display_fillRect(BUTTONS_BTN3_X_CORD, 0, BUTTONS_RECT_LENGTH,
                       BUTTONS_RECT_HEIGHT, DISPLAY_BLUE);
      // set text cursor to the center of the rectangle
      display_setCursor(BUTTONS_BTN3_TEXT_X_CORD, BUTTONS_TEXT_Y_CORD);
      // set text color black
      display_setTextColor(DISPLAY_BLACK);
      // write 'BTN3' to the screen
      display_println(BUTTONS_BTN3_TEXT);
    }
    // if the square was drawn and now button is not pressed
    else if ((prevVal & BUTTONS_BTN3_MASK) && !(currVal & BUTTONS_BTN3_MASK)) {
      // fill the square back in with black
      display_fillRect(BUTTONS_BTN3_X_CORD, 0, BUTTONS_RECT_LENGTH,
                       BUTTONS_RECT_HEIGHT, DISPLAY_BLACK);
    }
  }

  // fill the dsiplay with black
  display_fillScreen(DISPLAY_BLACK);
}