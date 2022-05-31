/*
    File: globals.c
    Author: Michael Norris
    Date: 5/31/2022
    Purpose: a file that stores the global sequence and has functions to return values when asked




*/

#include "globals.h"



//this array will hold our sewuence, only containing values 0-3
static uint8_t simonSequence[GLOBALS_MAX_FLASH_SEQUENCE] = {0};
//length of the sequence
static uint8_t sequenceSize = 0;
//for the current level in the sequence the user is at
static uint8_t currentSeqPos = 0;



// This is the length of the complete sequence at maximum length.
// You must copy the contents of the sequence[] array into the global variable
// that you maintain. Do not just grab the pointer as this will fail.
void globals_setSequence(const uint8_t sequence[], uint16_t length)
{   
    //iterate through the entire input array
    for (int i = 0; i < length; i++)
    {   //copy over the contents of the input array
        simonSequence[i] = sequence[i];
    }
    //set the lenght of the sequence
    sequenceSize = length;
}

// This returns the value of the sequence at the index.
uint8_t globals_getSequenceValue(uint16_t index)
{
    //give us the value at [index]
    return simonSequence[index];
}

// Retrieve the sequence length.
uint16_t globals_getSequenceLength()
{
    //give us the sequence length
    return sequenceSize;
}

// This is the length of the sequence that you are currently working on.
void globals_setSequenceIterationLength(uint16_t length)
{
    //returns the iteration length for the sequence
    currentSeqPos = length - 1;
}

// This is the length of the sequence that you are currently working on,
// not the maximum length but the interim length as
// the person playing the game works through the pattern one color at a time.
uint16_t globals_getSequenceIterationLength()
{
    return currentSeqPos;
}