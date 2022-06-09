/*
    File: scoreLivesTracker.c
    Author: Michael Norris
    Date: 6/9/2022
    Purpose: Tracks the score and lives



*/ 


#include <stdint.h>
#include <stdbool.h>

//the number of lives the player will start with
#define PLAYER_INIT_LIVES 3




//a number that will count the number of lives left for the player
static uint8_t livesRemeaining = PLAYER_INIT_LIVES; 