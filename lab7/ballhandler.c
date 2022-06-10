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
#include "touchHandler.h"
#include "scoreLivesTracker.h"

//these are the initial velocities of the ball when the game starts (it should be a 45 degree angle up to the right)
#define BALL_INIT_X_VELOCITY 5
#define BALL_INIT_Y_VELOCITY -5



//this flag will let us know if the machine is enabled or not
static uint8_t ballHandlerEnabled = 0;
//a flag for completion i guess i dont think this will use it tho
static uint8_t ballHandlerCompleted = 0;
//the actual ball object we are tracking
static struct objectProperties ball;
//this value will track if the ball went out of bounds
static uint8_t ballOutOfBounds = 0;
//this flag can be used to determine if the user is out of lives
static uint8_t isOutOfLives = 0;


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




bool ballHandler_isComplete()
{
    return ballHandlerCompleted;
}

// Standard tick function.
void ballHandler_tick()
{
    //switch statement for state transitions
    switch(currentState)
    {
        case st_init:
            if (ballHandlerEnabled)//if the machine has been enabled
            {
                //set the inital velocity of the ball
                ball.xVelocity = BALL_INIT_X_VELOCITY;
                ball.yVelocity = BALL_INIT_Y_VELOCITY;

                currentState = st_ball_move;//move to the ball moving state
            }
            break;
        case st_ball_move:
            if (!ballHandlerEnabled)
            {
                //reset to init state
                currentState = st_init;
            }
            //otherwise wait for the ball to go out of bounds
            else if (ballOutOfBounds)
            {
                //move to the out of play state
                currentState = st_ball_out_of_play;
            }
            break;
        case st_ball_out_of_play:
            if (!ballHandlerEnabled)
            {
                //reset to the init state
                currentState = st_init;
            }
            else//otherwise
            {
                //move right into the reset state
                currentState = st_ball_reset;
            }
            break;
        case st_ball_reset:

            break;
        case st_finished:

            break;
    }

    //switch statement for state actions
    switch(currentState)
    {
        case st_init:
            //reset all the relevant flags etc
            ballHandlerCompleted = 0;
            break;
        case st_ball_move:
            //should just be running the ball move function every tick while active
            ballHandler_moveBall(false);
            break;
        case st_ball_out_of_play:
            //just remove a life & erase the ball
            isOutOfLives = scoreLivesTracker_removeLife();
            breakoutDisplay_drawBall(ball.xPosition, ball.yPosition, true);
            break;
        case st_ball_reset:
            if (ballOutOfBounds)//just check this flag real quick to make sure we havent already moved the ball
            {
                //reset ball position and velocity
                ballHandler_moveBall(true);
                //reset the flag
                ballOutOfBounds = 0;
            }
            break;
        case st_finished:

            break;
    }
}

//this function handles the movement of the paddle from left to right taking the screen region
//into account and can even reset the paddle to its initial position if asked to 
void ballHandler_moveBall(bool reset)
{
    if (!reset)//if we arent resetting the location of the ball to the beginning
    {
        //we need to retrieve a paddle object to check the collision
        struct objectProperties paddleLocation = touchHandler_getPaddlePosition();
        //we need to check for tons of imminent collisions
        if(ball.xPosition - BALL_WIDTH_HEIGHT + ball.xVelocity <= TILE_FIRST_X_COORD || ball.xPosition + BALL_WIDTH_HEIGHT + ball.xVelocity >= DISPLAY_WIDTH - TILE_FIRST_X_COORD)//if it is hitting against the left or right wall
        {
            //we will just want to invert the xVelocity to get it to bounce off at a correct angle
            ball.xVelocity = ball.xVelocity * (-1);
        }
        if(ball.yPosition - BALL_WIDTH_HEIGHT + ball.yVelocity <= TILE_FIRST_Y_COORD)//if it is hitting the ceiling
        {
            //we should just be able to invert the x coordinate 
            ball.yVelocity = ball.yVelocity * (-1);
        }
        //it shouldn't hit the ceiling and the paddle at the same time
        //first we should check tha ball is curent above the paddle and will collide with it
        else if (((ball.yPosition + BALL_WIDTH_HEIGHT + ball.yVelocity >= paddleLocation.yPosition) && (ball.yPosition + BALL_WIDTH_HEIGHT <= paddleLocation.yPosition)))
        {
            //now we need to check that either the left or right end of the ball will collide with the paddle
            if (((ball.xPosition - BALL_WIDTH_HEIGHT + ball.xVelocity >= paddleLocation.xPosition) && (ball.xPosition - BALL_WIDTH_HEIGHT + ball.xVelocity <= paddleLocation.xPosition + PADDLE_WIDTH)) ||
            ((ball.xPosition + BALL_WIDTH_HEIGHT + ball.xVelocity >= paddleLocation.xPosition) && (ball.xPosition + BALL_WIDTH_HEIGHT + ball.xVelocity <= paddleLocation.xPosition + PADDLE_WIDTH)))
            {
                //we should just be able to invert the x coordinate 
                ball.yVelocity = ball.yVelocity * (-1);
            }
        }
        //it also should not hit the paddle and the bottom at the same time
        else if(ball.yPosition + BALL_WIDTH_HEIGHT + ball.yVelocity > DISPLAY_HEIGHT)
        {
            //ball went out of bounds
            ballOutOfBounds = 1;
        }

        //here we do the erasing and drawing of the ball
        if(ballOutOfBounds)
        {
            //do nothing here, i want the sm to deal with lives managment
            //scoreLivesTracker_removeLife();
        }
        else
        {
            //erase the old ball
            breakoutDisplay_drawBall(ball.xPosition, ball.yPosition, true);
            //adjust ball positon by the current velocity
            ball.xPosition = ball.xPosition + ball.xVelocity;
            ball.yPosition = ball.yPosition + ball.yVelocity;
            //redraw the ball in the new location
            breakoutDisplay_drawBall(ball.xPosition, ball.yPosition, false);

        }
    }
    else//if we are
    {
        //erase the ball from its old position
        breakoutDisplay_drawBall(ball.xPosition, ball.yPosition, true);
        //reset the balls velocity to zero
        ball.xVelocity = 0;
        ball.yVelocity = 0;
        //reset its coords to intial
        ball.xPosition = BALL_INIT_X_COORD;
        ball.yPosition = BALL_INIT_Y_COORD;
        //redraw the ball in the intial position
        breakoutDisplay_drawBall(ball.xPosition, ball.yPosition, false);
    }
}