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


int main()
{
    display_init();
    display_fillScreen(DISPLAY_BLACK);
    breakoutDisplay_drawPaddle((PADDLE_INIT_X_COORD), (PADDLE_INIT_Y_COORD), 0);
    breakoutDisplay_drawNewTiles();
    breakoutDisplay_drawBall(BALL_INIT_X_COORD, BALL_INIT_Y_COORD, false);
    breakoutDisplay_drawScore(100);


}




// Interrupt routine
void isr_function() 
{
  // Empty for flag method (flag set elsewhere)
}