/*
 * SimpleGame.c
 *
 *  Created on: Nov 4, 2013
 *      Author: C15Jason.Mossing
 */




#include <msp430g2553.h>
#include "SimpleGame.h"
#include "LCD/LCD.h"

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
