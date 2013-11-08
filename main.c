/*
 * Author: Jason Mossing
 * Description: This is the main for Lab 5.
 * This project initializes a SPI, LCD, buttons, and the interrupts for TIMERA and P2.
 * Then uses this information and one given about a player to play a simple game.
 */

#include <msp430.h> 
#include "LCD/LCD.h"
#include "buttons/button.h"
#include "SimpleGame.h"
#include "clkSpeed/clkSpeed.h"
#include "Random/rand.h"


void timerINIT();
void btnINIT();
void moveProperPlayer(char buttonToTest);
void Reset(char buttonToTest);
void testAndRespondToButtonPush(char buttonToTest);
void newGame();
void gameOver();

char btnPush = 0;
char timerCount = 0;
char player = 0;
char gameover = 0;
unsigned char mines[2];
unsigned int seed;

void clearTimer(){
	timerCount = 0;
	TACTL |= TACLR;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initSPI();
    initLCD();
    seed = prand(1234);
    newGame();

    btnINIT();
    timerINIT();
    __enable_interrupt();

    while(1){

    	//once player hits the bottom right corner he won
    	if(player == 0xC7){
    		TACTL &= ~TAIE;
    		clearLCD();
    		line1Cursor();
    		writeString("YOU");
    		line2Cursor();
    		writeString("WON!");
    		gameover = 1;
    		_delay_cycles(100000);
    	}
    	//action once a player hits a mine
    	if(didPlayerHitMine(player, mines) && gameover == 0){ //gameover == 0 because I want this to show once, and not to alternate between game over and kaboom
    		TACTL &= ~TAIE;
    		clearLCD();
    		line1Cursor();
    		writeString("KABOOM!");
    		line2Cursor();
    		writeString("Hit Mine");
    		_delay_cycles(200000);
    		gameOver();

    	}
    	//player looses once he doesn't move for 2 seconds
    	if(timerCount >= 4){
    		TACTL &= ~TAIE;
    		gameOver();
    	}

    }

	return 0;
}

void timerINIT(){
	//Stops TimerA
	TACTL &= ~(MC1|MC0);

	//Clear the timer
	TACTL |= TACLR;

	setspeed_1MHz(); //calibrate clock speed to 1MHz
	//divide the timer by 8
	TACTL |= TASSEL1;
	TACTL |= ID1|ID0;

	TACTL &= ~TAIFG; //clear the interrupt flag

	TACTL |= MC1; //continuous mode

	TACTL |= TAIE; //enable the interrupt
}

void btnINIT(){
	configureP2PinAsButton(BIT2);
	configureP2PinAsButton(BIT3);
	configureP2PinAsButton(BIT4);
	configureP2PinAsButton(BIT5);
	//sense falling edge
	P2IES |= BIT2|BIT3|BIT4|BIT5;
	//clear interrupt flag
	P2IFG &= ~ BIT2|BIT3|BIT4|BIT5;
	//enable all of the button interrupts
	P2IE |= BIT2|BIT3|BIT4|BIT5;
}

void testAndRespondToButtonPush(char buttonToTest)
{
    if (buttonToTest & P2IFG)
    {
        if (buttonToTest & P2IES)
        {
        	moveProperPlayer(buttonToTest);
            clearTimer();
        } else
        {
            debounce();
        }

        P2IES ^= buttonToTest;
        P2IFG &= ~buttonToTest;
    }
}

void moveProperPlayer(char buttonToTest){
	switch(buttonToTest){
		case BIT3:
			player = movePlayer(player,RIGHT);
			break;
		case BIT4:
			player = movePlayer(player,LEFT);
			break;
		case BIT5:
			player = movePlayer(player,UP);
			break;
		case BIT2:
			player = movePlayer(player,DOWN);
	}
}

void Reset(char buttonToTest){
    if (buttonToTest & P2IFG)
    {
        if (buttonToTest & P2IES)
        {
        	newGame();
            clearTimer();
            TACTL |= TAIE;
        } else
        {
            debounce();
        }

        P2IES ^= buttonToTest;
        P2IFG &= ~buttonToTest;
    }
}

void newGame(){
	gameover = 0;
	clearLCD();
	player = initPlayer();
	printPlayer(player);
	seed = generateMines(mines, seed);
	printMines(mines);
}

void gameOver(){
	clearLCD();
	line1Cursor();
	writeString("Game");
	line2Cursor();
	writeString("Over!");
	gameover = 1;
	_delay_cycles(100000);
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(){
	TACTL &= ~TAIFG;
	timerCount++;
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2_ISR(){
	//gameover determines whether to move the player or to restart the game
	if(gameover == 0){
	testAndRespondToButtonPush(BIT2);
	testAndRespondToButtonPush(BIT3);
	testAndRespondToButtonPush(BIT4);
	testAndRespondToButtonPush(BIT5);
	}
	else{
		Reset(BIT2);
		Reset(BIT3);
		Reset(BIT4);
		Reset(BIT5);
	}
}
