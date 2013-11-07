/*
 * SimpleGame.h
 *
 *  Created on: Nov 4, 2013
 *      Author: C15Jason.Mossing
 */

#ifndef SIMPLEGAME_H_
#define SIMPLEGAME_H_

#define RIGHT 1
#define LEFT 2
#define UP 3
#define DOWN 4

#define ROW_MASK 0x40

#define NUM_MINES 2

//
// Initializes player to starting position on board.
//
unsigned char initPlayer();

//
// Prints the player marker (*) at the player location passed in.
//
void printPlayer(unsigned char player);

//
// Clears the location passed in.
//
void clearPlayer(unsigned char player);

//
// Given a player's current position and a direction, returns an updated player position.
//
unsigned char movePlayer(unsigned char player, unsigned char direction);

//
// Returns true if the position passed in is the winning game position.
//
char didPlayerWin(unsigned char player);

 //Functions which may prove useful in achieving A-Functionality.

char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]);
int generateMines(unsigned char mines[NUM_MINES], int seed);
void printMines(unsigned char mines[NUM_MINES]);



#endif /* SIMPLEGAME_H_ */
