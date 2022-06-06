
#include "switches.h"
#include "leds.h"
#include "xil_io.h"
#include "xparameters.h"

// define the base address
#define SWITCHES_GPIO_DEVICE_BASE_ADDRESS XPAR_SLIDE_SWITCHES_BASEADDR
// tri state offset is BASE_ADDR + 4
#define SWITCHES_TRI_STATE_OFFSET 0X04
// mask to set mode to input
#define SWITCHES_IS_INPUT 0xF
// mask to check for if all swiches are flipped
#define SWITCHES_ALL_FLIPPED 0xF

// helper function that reads in from the GPIO Switch register using an offset
int32_t switches_readGpioRegister(int32_t offset) {
  // returns the value of the address using the low level Xilinx call
  return Xil_In32(SWITCHES_GPIO_DEVICE_BASE_ADDRESS + offset);
}

// helper function to write a value to the specified address using an offset
void switches_writeGpioRegister(int32_t offset, int32_t value) {
  // Xilinx call that writes the value to the specified location
  Xil_Out32(SWITCHES_GPIO_DEVICE_BASE_ADDRESS + offset, value);
}

// Initializes the SWITCHES driver software and hardware. Returns one of the
// STATUS values defined above.
int32_t switches_init() {
  // get the current value of the tri state register
  uint32_t triStateValue = switches_readGpioRegister(SWITCHES_TRI_STATE_OFFSET);
  // set up the GPIO register to be an input by setting the low 4 bits to be 1
  // and then writing it back
  triStateValue = triStateValue | SWITCHES_IS_INPUT;
  switches_writeGpioRegister(SWITCHES_TRI_STATE_OFFSET, triStateValue);
  // return the ok signal
  return SWITCHES_INIT_STATUS_OK;
}

// Returns the current value of all 4 switches as the lower 4 bits of the
// returned value. bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
int32_t switches_read() {
  // read in the values of the 4 switchess and return it
  uint32_t switchesInputValues = switches_readGpioRegister(0);
  return switchesInputValues;
}

// Runs a test of the switches. As you slide the switches, LEDs directly above
// the switches will illuminate. The test will run until all switches are slid
// upwards. When all 4 slide switches are slid upward, this function will
// return.
void switches_runTest() {
  // initailize the switches
  switches_init();
  // initialize the leds
  leds_init(0);
  // variable to store switch values
  uint32_t switches = 0;
  // While all switches are not flipped do this
  while (switches ^ SWITCHES_ALL_FLIPPED) {
    // Read switches and update the stored variable
    switches = switches_read();
    // Write the switch value to the leds
    leds_write(switches & SWITCHES_ALL_FLIPPED);
  }
  // Turn all LEDs off
  leds_write(!switches);
}