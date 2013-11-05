#include <msp430.h> 
#include "LCD/LCD.h"
#include "buttons/button.h"
#include "SimpleGame.h"
#include "clkSpeed/clkSpeed.h"

/*
 * main.c
 */
void timerINIT();
void btnINIT();
void moveProperPlayer(char buttonToTest);
void Reset(char buttonToTest);

char btnPush = 0;
char timerCount = 0;
char player = 0;
char gameover = 0;

void clearTimer(){
	timerCount = 0;
	TACTL |= TACLR;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    player = initPlayer();
    initSPI();
    initLCD();
    clearLCD();
    printPlayer(player);
    btnINIT();
    timerINIT();
    __enable_interrupt();

    while(1){

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
    	if(timerCount >= 4){
    		TACTL &= ~TAIE;
    		clearLCD();
    		line1Cursor();
    		writeString("Game");
    		line2Cursor();
    		writeString("Over!");
    		gameover = 1;
    		_delay_cycles(100000);
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
        	gameover = 0;
        	clearLCD();
        	player = initPlayer();
        	printPlayer(player);
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
