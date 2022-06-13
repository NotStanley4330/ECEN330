/*
    File: tileHandler.h
    Author: Michael Norris
    Date: 6/10/2022
    Purpose: This deals with tracking the bricks that exist and need to be destroyed
             I will have a 2d array set up to track each tile 



*/ 

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "breakoutGlobals.h"
#include "tileHandler.h"
#include "breakoutDisplay.h"
#include "scoreLivesTracker.h"

// struct tileItem
// {
//     uint16_t xPosition;
//     uint16_t yPosition;
//     uint8_t isDestroyed;
//     uint16_t colorCode;
// };

//this will be a 2d struct that tracks all the tiles on the board
static struct tileItem tiles[TILE_ROW_NUM][TILE_COLUMN_NUM];



//this function will place the tile and set it up in the array
void tileHandler_createTile(uint8_t row, uint8_t column, int16_t xCoord, int16_t yCoord, uint16_t colorCode, bool isDestroyed)
{
    //assign all the properties to the apropriate tile
    tiles[row][column].xPosition = xCoord;
    tiles[row][column].yPosition = yCoord;
    tiles[row][column].colorCode = colorCode;
    tiles[row][column].isDestroyed = isDestroyed;
    //printf("created tile in row: %i column: %i with doesExist value of %i", row, column, isDestroyed);
    if (!isDestroyed)//if the tile actually existst draw it
    {
        breakoutDisplay_drawTile(xCoord, yCoord, colorCode, false);
    }
}

//this function will check for a tile existing based on coordinates
bool tileHandler_checkForTile(int16_t xCoord, int16_t yCoord)
{
    int8_t row = TILE_ROW_NUM;
    int8_t column = TILE_COLUMN_NUM;;//these will store the variables of the tile that it was found colliding with
    //first check row then tile, this should be more efficient then checkign every single tile
    for(int i = 0; i < TILE_ROW_NUM; i++)
    {
        //if we have found the row we are in
        printf("current values are ycoord: %i, tiles.yPos: %i\n", yCoord, tiles[i][1].yPosition);
        if (yCoord >= tiles[i][1].yPosition && yCoord <= (tiles[i][1].yPosition + TILE_HEIGHT + TILE_SPACER_WIDTH))
        {
            //printf("row = %i\n", row);
            row = (int8_t)i;
            break;
        }
    }
    //now we check for what tile it is in that row
    for (int j = 0; j < TILE_COLUMN_NUM; ++j)
    {
        //printf("j = %i\n", j);
        //if we found it is between these two values
        if (xCoord >= tiles[row][j].xPosition && xCoord <= (tiles[row][j].xPosition + TILE_WIDTH))
        {
            column = (int8_t)j;
            //printf("column = %i\n", column);
            break;
        }
    }

    

    //now check that the tile still exists (hasnt been destroyed/was generated in the round)
    if (!(tiles[row][column].isDestroyed))
    {
        //printf("destroying tile at row: %i column:%i\n", row, column);
        //destroy the tile and tell the ball bouncing function that it exists

        //try to make sure it destroys the tile below first
        
            tileHandler_destroyTile(row, column);
        
        
        return true;
    }
    else//if it has been destroyed already let the ballHandler know
    {
        return false;
    }
}

//this function will destroy the tile
void tileHandler_destroyTile(int8_t row, int8_t column)
{
    printf("destroying tile at row: %i column: %i\n", row, column);
    //erase the tile from existence
    if(column > TILE_COLUMN_NUM || row > TILE_ROW_NUM)//exit the function early if we overran our column or tile numbers
    {
        return;
    }
    breakoutDisplay_drawTile(tiles[row][column].xPosition, tiles[row][column].yPosition, DISPLAY_BLACK, true);
    //set is destroyed to true
    tiles[row][column].isDestroyed = 1;
    //increment the score
    scoreLivesTracker_addTileScore();
}