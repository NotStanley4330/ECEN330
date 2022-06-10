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
#include "tileHandler.h"
#include "breakoutDisplay.h"



//this will be a 2d struct that tracks all the tiles on the board
static struct tileItem tiles[TILE_ROW_NUM][TILE_ROW_NUM];



//this function will place the tile and set it up in the array
void tileHandler_createTile(uint8_t row, uint8_t column, int16_t xCoord, int16_t yCoord, uint16_t colorCode, bool isDestroyed)
{
    //assign all the properties to the apropriate tile
    tileItem[row][column].xPosition = xCoord;
    tileItem[row][column].yPosition = yCoord;
    tileItem[row][column].colorCode = colorCode;
    tileItem[row][column].isDestroyed = isDestroyed;
    if (!isDestroyed)//if the tile actually existst draw it
    {
        breakoutDisplay_drawTile(xCoord, yCoord, colorCode, false);
    }
}

//this function will check for a tile existing based on coordinates
bool tileHandler_checkForTile(int16_t xCoord, int16_t yCoord);
{
    uint8_t row, column;//these will store the variables of the tile that it was found colliding with
    //first check row then tile, this should be more efficient then checkign every single tile
    for(uint8_t i = 0; i < TILE_ROW_NUM; i++)
    {
        //if we have found the row we are in
        if (yCoord >= tileItem[i][0].yPosition && yCoord <= (tileItem[i][0].yPosition + TILE_HEIGHT))
        {
            row = i;
            break;
        }
    }
    //now we check for what tile it is in that row
    for (uint8_t i = 0; i < TILE_COLUMN_NUM; i++)
    {
        //if we found it is between these two values
        if (xCoord >= tileItem[row][i].xPosition && xCoord <= tileItem[row][i].xPosition)
        {
            column = i;
            break;
        }
    }
}

//this function will destroy the tile
void DestroyTile(uint8_t row, uint8_t column);