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

char btnPush = 0;
char timerCount = 0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    char player = initPlayer();
    initLCD();
    clearLCD();
    initSPI();
    btnINIT();
    timerINIT();
    __enable_interrupt();

    while(1){
    	//check direction and if it is falling edge
    	//move if passed
    	//check to see if won
    	if(timerCount >= 4){
    		line1Cursor();
    		writeString("Game    ");

    		line2Cursor();
    		writeString("Over!  ");
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
	configureP2PinAsButton(2);
	configureP2PinAsButton(3);
	configureP2PinAsButton(4);
	configureP2PinAsButton(5);
	P2IES |= BIT2|BIT3|BIT4|BIT5;
	P2IFG &= ~ BIT2|BIT3|BIT4|BIT5;
	P2IE |= BIT2|BIT3|BIT4|BIT5;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(){
	TACTL &= ~TAIFG;
	timerCount++;
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2_ISR(){
	/*
	 * if statements for checking the flag bit
	 * within bits check to see if it is checking falling or rising edge.
	 * toggle check of rising and falling edge.
	 */

}
