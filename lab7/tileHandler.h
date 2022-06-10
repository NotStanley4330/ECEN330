/*
    File: tileHandler.h
    Author: Michael Norris
    Date: 6/10/2022
    Purpose: This deals with tracking the bricks that exist and need to be destroyed
             I will have a 2d array set up to track each tile 



*/ 

#include <stdint.h>
#include <stdbool.h>
#include "breakoutGlobals.h"




//this function will place the tile and set it up in the array
void tileHandler_createTile(uint8_t row, uint8_t column, int16_t xCoord, int16_t yCoord, uint16_t colorCode, bool isDestroyed);

//this function will check for a tile existing based on coordinates
bool tileHandler_checkForTile(int16_t xCoord, int16_t yCoord);

//this function will destroy the tile
void DestroyTile(uint8_t row, uint8_t column);