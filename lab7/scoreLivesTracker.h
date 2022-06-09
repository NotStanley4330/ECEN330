/*
    File: scoreLivesTracker.h
    Author: Michael Norris
    Date: 6/9/2022
    Purpose: Tracks the score and lives



*/ 
#ifndef SCORELIVESTRACKER_H_
#define SCORELIVESTRACKER_H_

#include "breakoutGlobals.h"
#include <stdint.h>
#include <stdbool.h>



//this funtion is for adding & reprinting when a tile is broken
void scoreLivesTracker_addTileScore();

//this function is for adding and reprinting the score when a level is cleared
void scoreLivesTracker_addLevelScore();

//this function is for adding a life
void scoreLivesTracker_addLife();

//this funtion is for subtracting a life, if there are already no lives you should return a 
//bool of true that lives have run out
bool scoreLivesTracker_removeLife();


#endif //SCORELIVESTRACKER_H_