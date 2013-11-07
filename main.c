#include <msp430.h> 
#include "LCD/LCD.h"
#include "buttons/button.h"
#include "SimpleGame.h"
#include "clkSpeed/clkSpeed.h"
#include "Random/rand.h"

/*
 * main.c
 */
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
int seed;

void clearTimer(){
	timerCount = 0;
	TACTL |= TACLR;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initSPI();
    initLCD();
    seed = rand();
    newGame();

    btnINIT();
    timerINIT();
    __enable_interrupt();

    while(1){
    	//every second not moved the mines will move.
    	/*if(timerCount == 2){
    		//pause the timer
    		TACTL &= ~(MC1|MC0);
    		seed = generateMines(mines, seed);
    		//continue the timer
    		TACTL |= MC1;
    	}*/
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
    	if(didPlayerHitMine(player, mines) && gameover == 0){
    		TACTL &= ~TAIE;
    		clearLCD();
    		line1Cursor();
    		writeString("KABOOM!");
    		line2Cursor();
    		writeString("Hit Mine");
    		_delay_cycles(200000);
    		gameOver();

    	}
    	if(timerCount >= 4){
    		TACTL &= ~TAIE;
    		gameOver();
    	}

    }

	return 0;
}

void timerINIT(){
	TACTL &= ~(MC1|MC0);

	TACTL |= TACLR;

	setspeed_1MHz();
	TACTL |= TASSEL1;
	TACTL |= ID1|ID0;

	TACTL &= ~TAIFG;

	TACTL |= MC1;

	TACTL |= TAIE;
}

void btnINIT(){
	configureP2PinAsButton(BIT2);
	configureP2PinAsButton(BIT3);
	configureP2PinAsButton(BIT4);
	configureP2PinAsButton(BIT5);
	P2IES |= BIT2|BIT3|BIT4|BIT5;
	P2IFG &= ~ BIT2|BIT3|BIT4|BIT5;
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
