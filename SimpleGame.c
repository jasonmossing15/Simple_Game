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
	switch (direction) {
		case UP:
			player ^= BIT6;
			break;
		case DOWN:
			player ^= BIT6;
			break;
		case RIGHT:
			player ++;
			if (player > 0x87 & player < 0xC0){
				player = 0xC0;
			}
			break;
		case LEFT:
			player --;
			if(player < 0x80){
				player = 0x80;
			}
			if(player < 0xC0 & player > 0x87){
				player = 0x87;
			}
	}
	return player;
}

char didPlayerWin(unsigned char player){
	return player == 0xC7;
}
