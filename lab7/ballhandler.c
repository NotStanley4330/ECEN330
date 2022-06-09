/*
    File: ballHandler.c
    Author: Michael Norris
    Date: 6/9/2022
    Purpose: This deals with movement of the ball around the screen and collisions with certain objects



*/ 
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "ballHandler.h"
#include "breakoutGlobals.h"
#include "breakoutDisplay.h"



//this flag will let us know if the machine is enabled or not
static uint8_t ballHandlerEnabled = 0;
//a flag for completion i guess i dont think this will use it tho
static uint8_t ballHnadlerCompleted = 0;
//the actual ball object we are tracking
static struct objectProperties ball;

//the actual states for the machine
enum ballHandler_st_t {
  st_init,        // the initial state of the machine
  st_ball_move, // state for the ball to move around in
  st_ball_out_of_play,      // a state for what happens if the ball is out of play
  st_ball_reset,  // a state that resets the ball to the inital position
  st_finished     // ending state of the machine

};
static enum ballHandler_st_t currentState;


// Turn on the state machine.
void ballHandler_enable()
{
    ballHandlerEnabled = 1;
}

// Turn off the state machine.
void ballHandler_disable()
{
    ballHandlerEnabled = 0;
}

// Standard init function.
void ballHandler_init()
{
    //set the current state to the initial state and the positions to their initial location
    currentState = st_init;
    ball.xPosition = BALL_INIT_X_COORD;
    ball.yPosition = BALL_INIT_Y_COORD;
    ball.xVelocity = 0;
    ball.yVelocity = 0;
}



// Returns wether or not the screen has been touched
bool ballHandler_isComplete()
{

}

// Standard tick function.
void ballHandler_tick()
{

}

//this function handles the movement of the paddle from left to right taking the screen region
//into account and can even reset the paddle to its initial position if asked to 
void ballHandler_movePaddle(bool reset)
{

}