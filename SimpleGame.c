/*
 * SimpleGame.c
 *
 *  Created on: Nov 4, 2013
 *      Author: C15Jason.Mossing
 */




#include <msp430g2553.h>
#include "SimpleGame.h"
#include "LCD/LCD.h"
#include "Random/rand.h"

char isGameWinnable(unsigned char mines[NUM_MINES]);

unsigned char initPlayer(){
	return 0x80;
}

void printPlayer(unsigned char player){
	writeCommandByte(player);
	writeDataByte('*');
}

void clearPlayer(unsigned char player){
	writeCommandByte(player);
	writeDataByte(' ');
}

unsigned char movePlayer(unsigned char player, unsigned char direction){
	clearPlayer(player);
	switch (direction) {
		case UP:
			player &= ~BIT6;
			break;
		case DOWN:
			player |= BIT6;
			break;
		case RIGHT:
			if (player < 0x87 || player >= 0xC0){
				player++;
			}
			break;
		case LEFT:
			if ((player & 0x0f)>0)
				player--;
	}
	printPlayer(player);
	return player;
}

char didPlayerWin(unsigned char player){
	return player == 0xC7;
}

char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]){
	int i, hit = 0;
	for(i = 0; i < NUM_MINES; i++){
		if(player == mines[i]){
			hit = 1;
		}
	}
	return hit;
}

int generateMines(unsigned char mines[NUM_MINES],unsigned int seed){
	unsigned char winnable = 0;
	while(!winnable){
		seed = prand(seed);
		mines[0] = 0x81 + (seed % 7);

		seed = prand(seed);
		mines[1] = 0xC0 + (seed % 7);

		winnable = isGameWinnable(mines);
	}
	return seed;
}
void printMines(unsigned char mines[NUM_MINES]){
	int i;
	for(i = 0; i < NUM_MINES; i++){
		writeCommandByte(mines[i]);
		writeDataByte('X');
	}
}

char isGameWinnable(unsigned char mines[NUM_MINES]){
	char top = mines[0];
	char bottom = mines[1];
	//& with 0x0f in order to check only the lower nibble
	top = top & 0x0f;
	bottom = bottom & 0x0f;

	//checks up&down and the diagonals, if either of these occur the game is unwinnable
	return !(top == bottom || top+1 == bottom || top-1 == bottom);
}

