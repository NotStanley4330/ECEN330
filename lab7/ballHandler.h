/*
    File: ballHandler.h
    Author: Michael Norris
    Date: 6/9/2022
    Purpose: This deals with movement of the ball around the screen and collisions with certain objects



*/ 


#ifndef BALLHANDLER_H_
#define BALLHANDLER_H_

#include <stdbool.h>
#include <stdint.h>

// Turn on the state machine.
void ballHandler_enable();

// Turn off the state machine.
void ballHandler_disable();

// Standard init function.
void ballHandler_init();



// Returns wether or not the ball handler is done with its stuff or something idk
bool ballHandler_isComplete();

// Standard tick function.
void ballHandler_tick();

//this function handles the movemement of the ball including collision
//detection with the walls, tiles, and paddles
void ballHandler_moveBall(bool reset);



#endif //BALLHANDLER_H_