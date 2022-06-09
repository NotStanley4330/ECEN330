/*
    Project: Breakout for ZYBO board
    File: main.c
    Author: Michael Norris
    Date: 6/7/2022
    Purpose: Run the main loop for the Breakout






*/


#include <stdio.h>
#include <stdbool.h>

#include "buttons.h"
#include "config.h"
#include "display.h"
#include "interrupts.h"
#include "intervalTimer.h"
#include "leds.h"
#include "switches.h"
#include "utils.h"
#include "paddleControl.h"
#include "breakoutDisplay.h"
#include "xparameters.h"
#include "touchHandler.h"


// Compute the timer clock freq.
#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
// Compute timer load value.
#define TIMER_LOAD_VALUE ((CONFIG_TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)


void init_all()
{
  display_init();
  touchHandler_init();
}

void tickAll()
{
  touchHandler_tick();
}

int main()
{
    
    display_fillScreen(DISPLAY_BLACK);
    breakoutDisplay_drawPaddle((PADDLE_INIT_X_COORD), (PADDLE_INIT_Y_COORD), 0);
    breakoutDisplay_drawNewTiles();
    breakoutDisplay_drawBall(BALL_INIT_X_COORD, BALL_INIT_Y_COORD, false);
    breakoutDisplay_drawScore(100, false);


     // Init all interrupts (but does not enable the interrupts at the devices).
  // Prints an error message if an internal failure occurs because the argument
  // = true.
  interrupts_initAll(true);
  interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
  interrupts_enableTimerGlobalInts();
  // Keep track of your personal interrupt count. Want to make sure that you
  // don't miss any interrupts.
  int32_t personalInterruptCount = 0;
  // Start the private ARM timer running.
  interrupts_startArmPrivateTimer();
  // Enable interrupts at the ARM.
  interrupts_enableArmInts();
  while (1) {
    if (interrupts_isrFlagGlobal) {
      // Count ticks.
      personalInterruptCount++;
      tickAll();
      interrupts_isrFlagGlobal = 0;
      // if (personalInterruptCount >= MAX_INTERRUPT_COUNT)
      //   break;
      utils_sleep();
    }
  }


}




// Interrupt routine
void isr_function() 
{
  // Empty for flag method (flag set elsewhere)
}