/*
    File: scoreLivesTracker.c
    Author: Michael Norris
    Date: 6/9/2022
    Purpose: Tracks the score and lives



*/ 


#include <stdint.h>
#include <stdbool.h>
#include "breakoutDisplay.h"
#include "scoreLivesTracker.h"

//the number of lives the player will start with
#define PLAYER_INIT_LIVES 3
//the number of points a player needs ot get before getting an extra life
#define EXTRA_LIFE_THERSHOLD 10000
//the amount you should incrememt score by when breaking a tile
#define TILE_BREAK_SCORE 100
//the amount you should get for clearing a level
#define LEVEL_CLEAR_SCORE 1000





//a number that will count the number of lives left for the player
static uint8_t livesRemaining = PLAYER_INIT_LIVES; 
//the actual score
static uint32_t playerScore = 0;





//this funtion is for adding & reprinting when a tile is broken
void scoreLivesTracker_addTileScore()
{   
    //erase the old score
    breakoutDisplay_drawScore(playerScore, true);
    //add the additional score
    playerScore = playerScore + TILE_BREAK_SCORE;
    //redraw the new score
    breakoutDisplay_drawScore(playerScore, false);
    if (!(playerScore % EXTRA_LIFE_THERSHOLD))//if we have hit a multiple of EXTRA_LIFE_SCORE
    {
        scoreLivesTracker_addLife();
    }
}

//this function is for adding and reprinting the score when a level is cleared
void scoreLivesTracker_addLevelScore()
{
    //erase the old score
    breakoutDisplay_drawScore(playerScore, true);
    //add the additional score
    playerScore = playerScore + LEVEL_CLEAR_SCORE;
    //redraw the new score
    breakoutDisplay_drawScore(playerScore, false);
    if (!(playerScore % EXTRA_LIFE_THERSHOLD))//if we have hit a multiple of EXTRA_LIFE_SCORE
    {
        scoreLivesTracker_addLife();
    }
}

//this function is for adding a life
void scoreLivesTracker_addLife()
{
    //first we wanna erase the old lives value
    breakoutDisplay_drawLives(livesRemaining, true);
    //just add a life
    livesRemaining++;
    //now draw the new lives value
    breakoutDisplay_drawLives(livesRemaining, false);
}

//this funtion is for subtracting a life, if there are already no lives you should return a 
//bool of true that lives have run out
bool scoreLivesTracker_removeLife()
{
    if (livesRemaining > 0)
    {
        //first we wanna erase the old lives value
        breakoutDisplay_drawLives(livesRemaining, true);
        livesRemaining--;//now decrement it
        //now draw the new lives value
        breakoutDisplay_drawLives(livesRemaining, false);
        return false;//the player is not out of lives if they have at least one left
    }
    else
    {
        return true;//they are out of lives and the game should handle that
    }
}


//this function simply takes the lives counter and resets it to 3
void scoreLivesTracker_resetLives()
{
    //first we wanna erase the old lives value
        breakoutDisplay_drawLives(livesRemaining, true);
        livesRemaining = PLAYER_INIT_LIVES;//now reset it
        //now draw the new lives value
        breakoutDisplay_drawLives(livesRemaining, false);
}

//this function takes the score and resets it to zero
void socreLivesTracker_resetScore()
{
    //erase the old score
    breakoutDisplay_drawScore(playerScore, true);
    //reset to zero
    playerScore = 0;
    //redraw the new score
    breakoutDisplay_drawScore(playerScore, false);
}