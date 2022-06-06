

#include "intervalTimer.h"
#include "xil_io.h"
#include "xparameters.h"
#include <stdio.h>

// control/status register 0
#define INTERVAL_TIMER_TCSR0 0x0
// load register 0
#define INTERVAL_TIMER_TLR0 0x04
// timer/counter register 0
#define INTERVAL_TIMER_TCR0 0x08
// control/status register 1
#define INTERVAL_TIMER_TCSR1 0x10
// load register 1
#define INTERVAL_TIMER_TLR1 0x14
// timer/counter register 1
#define INTERVAL_TIMER_TCR1 0x18
// value to clear a timer
#define INTERVAL_TIMER_CLEAR 0x0
// value to cause a timer to load the load register
#define INTERVAL_TIMER_LOAD0 0x20
//// bit to set to cascade the two timers
#define INTERVAL_TIMER_CASCADE 0x800
// hex value to set the timer to start
#define INTERVAL_TIMER_ENT0 0x80
// maximum timer number for for loops
#define INTERVAL_TIMER_MAX_TIMER_COUNTER_VAL 2
// the frequency of the timers to convert cycles to seconds
#define INTERVAL_TIMER_CLOCK_FREQUENCY XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ
// define the left shift value for concatonation
#define INTERVAL_TIMER_SHIFT_32 32
// the number of the timers
#define INTERVAL_TIMER_NUM_0 0
#define INTERVAL_TIMER_NUM_1 1
#define INTERVAL_TIMER_NUM_2 2

// helper function that reads in from the timer address
int32_t intervalTimer_readRegister(uint32_t timerAddr, int32_t offset) {
  // returns the value of the address using the low level Xilinx call
  return Xil_In32(timerAddr + offset);
}

// helper function to write a value to the specified address with offset
void intervalTimer_writeRegister(uint32_t timerAddr, int32_t offset,
                                 int32_t value) {
  // Xilinx call that writes the value to the address
  Xil_Out32(timerAddr + offset, value);
}
// helper funtcion to set a bit or bits in a register without affecting other
// ones
void intervalTimer_setRegisterValue(uint32_t timerAddr, int32_t offset,
                                    int32_t value) {
  // get the current value of the register using given address
  uint32_t regValue = intervalTimer_readRegister(timerAddr, offset);
  // Set the bits without messing with other ones
  regValue = regValue | value;
  // write this value back to the given register
  intervalTimer_writeRegister(timerAddr, offset, regValue);
}

// use this to clear out a bit or bits in a register back to 0 without affecting
// other ones
void intervalTimer_clearRegisterValue(uint32_t timerAddr, int32_t offset,
                                      int32_t value) {
  // get the current value of the register using given address
  uint32_t regValue = intervalTimer_readRegister(timerAddr, offset);
  // Clear the bits by anding with the inverse of the value
  regValue = regValue & ~value;
  // write this value back to the given register
  intervalTimer_writeRegister(timerAddr, offset, regValue);
}

// Returns an address based on the timerNumber giver
uint32_t intervalTimer_getTimerAddress(uint32_t timerNumber) {
  // a series of if else statemnts to return the address based on the timer
  // number
  if (timerNumber == INTERVAL_TIMER_NUM_0) {
    // return timer 0 base address
    return XPAR_AXI_TIMER_0_BASEADDR;
  } else if (timerNumber == INTERVAL_TIMER_NUM_1) {
    // return timer 1 base address
    return XPAR_AXI_TIMER_1_BASEADDR;
  } else if (timerNumber == INTERVAL_TIMER_NUM_2) {
    // return timer 2 base address
    return XPAR_AXI_TIMER_2_BASEADDR;
  } else // a bad number for the timer was passed in
  {
    return INTERVAL_TIMER_STATUS_FAIL;
  }
}

// read both sections of the cascaded timer, concatonate into a 64 bit value and
// return it
uint64_t intervalTimer_getTimerValue(uint32_t timerNumber) {
  // get the address of the timer
  uint32_t timerAddr = intervalTimer_getTimerAddress(timerNumber);
  // store the lower and upper 32 bit numbers
  uint32_t lowerCounter =
      intervalTimer_readRegister(timerAddr, INTERVAL_TIMER_TCR0);
  // store the upper value as a 64 bit and shift it right 32 bits so we can
  // concatonate them
  uint64_t upperCounter =
      intervalTimer_readRegister(timerAddr, INTERVAL_TIMER_TCR1);
  uint64_t totalCounter = upperCounter << INTERVAL_TIMER_SHIFT_32;
  // add the lower to the lower 32 bits
  totalCounter += lowerCounter;
  // return the total as a single 64 bit value
  return totalCounter;
}

// You must initialize the timers before you use them the first time.
// It is generally only called once but should not cause an error if it
// is called multiple times.
// timerNumber indicates which timer should be initialized.
// returns INTERVAL_TIMER_STATUS_OK if successful, some other value otherwise.
intervalTimer_status_t intervalTimer_init(uint32_t timerNumber) {
  // get the address of the timer to be initialized
  uint32_t timerAddr = intervalTimer_getTimerAddress(timerNumber);
  // as long as we got a valid address back initialize the timer
  if (timerAddr != 0) {
    // write a zero to TCSR0
    intervalTimer_writeRegister(timerAddr, INTERVAL_TIMER_TCSR0,
                                INTERVAL_TIMER_CLEAR);
    // set the cascade bit in TCSR0
    intervalTimer_setRegisterValue(timerAddr, INTERVAL_TIMER_TCSR0,
                                   INTERVAL_TIMER_CASCADE);
    // write a zero to TCSR1
    intervalTimer_writeRegister(timerAddr, INTERVAL_TIMER_TCSR1,
                                INTERVAL_TIMER_CLEAR);
    // if everything worked, return INTERVALTIMER_SUCCESS
    return INTERVAL_TIMER_STATUS_OK;
  } else // if we get a bad address number return a fail
  {
    // return fail status
    return INTERVAL_TIMER_STATUS_FAIL;
  }
}

// This is a convenience function that initializes all interval timers.
// Simply calls intervalTimer_init() on all timers.
// returns INTERVAL_TIMER_STATUS_OK if successful, some other value otherwise.
intervalTimer_status_t intervalTimer_initAll() {
  // for counters 0-2 to do this:
  for (uint32_t i = 0; i <= INTERVAL_TIMER_MAX_TIMER_COUNTER_VAL; i++) {
    // initialize counter (i)
    intervalTimer_init(i);
  }
  // if everything suceeded, return INTERVALTIMER_SUCCESS
  return INTERVAL_TIMER_STATUS_OK;
}

// This function starts the interval timer running.
// If the interval timer is already running, this function does nothing.
// timerNumber indicates which timer should start running.
// valid timers are 0-2
void intervalTimer_start(uint32_t timerNumber) {
  // get the current timer address
  uint32_t timerAddr = intervalTimer_getTimerAddress(timerNumber);
  // set the incrementing bit in the timer register
  intervalTimer_setRegisterValue(timerAddr, INTERVAL_TIMER_TCSR0,
                                 INTERVAL_TIMER_ENT0);
}

// This function stops a running interval timer.
// If the interval time is currently stopped, this function does nothing.
// timerNumber indicates which timer should stop running.
////valid timers are 0-2
void intervalTimer_stop(uint32_t timerNumber) {
  // get the current timer address
  uint32_t timerAddr = intervalTimer_getTimerAddress(timerNumber);
  // clear the incrementing bit in the timer register
  intervalTimer_clearRegisterValue(timerAddr, INTERVAL_TIMER_TCSR0,
                                   INTERVAL_TIMER_ENT0);
}

// This function is called whenever you want to reuse an interval timer.
// For example, say the interval timer has been used in the past, the user
// will call intervalTimer_reset() prior to calling intervalTimer_start().
// timerNumber indicates which timer should reset.
// valid timers are 0-2
void intervalTimer_reset(uint32_t timerNumber) {
  // get the current timer address
  uint32_t timerAddr = intervalTimer_getTimerAddress(timerNumber);
  // set the load register 0 to 0
  intervalTimer_writeRegister(timerAddr, INTERVAL_TIMER_TLR0,
                              INTERVAL_TIMER_CLEAR);
  // cause the load register to actually load in by setting the LOAD0 bit of
  // TCSR0
  intervalTimer_setRegisterValue(timerAddr, INTERVAL_TIMER_TCSR0,
                                 INTERVAL_TIMER_LOAD0);
  // stop the loading by clearing that bit
  intervalTimer_clearRegisterValue(timerAddr, INTERVAL_TIMER_TCSR0,
                                   INTERVAL_TIMER_LOAD0);

  // repeat all the same steps for the second half of the timer
  intervalTimer_writeRegister(timerAddr, INTERVAL_TIMER_TLR1,
                              INTERVAL_TIMER_CLEAR);
  intervalTimer_setRegisterValue(timerAddr, INTERVAL_TIMER_TCSR1,
                                 INTERVAL_TIMER_LOAD0);
  intervalTimer_clearRegisterValue(timerAddr, INTERVAL_TIMER_TCSR1,
                                   INTERVAL_TIMER_LOAD0);
}

// Convenience function for intervalTimer_reset().
// Simply calls intervalTimer_reset() on all timers.
void intervalTimer_resetAll() {
  // for counters 0-2 to do this:
  for (uint32_t i = 0; i <= INTERVAL_TIMER_MAX_TIMER_COUNTER_VAL; i++) {
    // initialize counter (i)
    intervalTimer_reset(i);
  }
}

// Runs a test on a single timer as indicated by the timerNumber argument.
// Returns INTERVAL_TIMER_STATUS_OK if successful, something else otherwise.
intervalTimer_status_t intervalTimer_test(uint32_t timerNumber);

// Convenience function that invokes test on all interval timers.
// Returns INTERVAL_TIMER_STATUS_OK if successful, something else otherwise.
intervalTimer_status_t intervalTimer_testAll();

// Use this function to ascertain how long a given timer has been running.
// Note that it should not be an error to call this function on a running timer
// though it usually makes more sense to call this after intervalTimer_stop()
// has been called. The timerNumber argument determines which timer is read.
double intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber) {
  double seconds = (double)intervalTimer_getTimerValue(timerNumber) /
                   INTERVAL_TIMER_CLOCK_FREQUENCY;

  return seconds;
}