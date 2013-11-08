# Lab 5 Simple Game

This repository creates a simple game where the objective is to move the player from the top left to the bottom right of the screen. The screen used for this code is a 2x8 LCD screen hooked up to the infamous Geek Box.

## The Code

There are 3 main aspects to making this code work:
- SPI/LCD
- Buttons
- Interrupts
The first two aspects were first tested in Lab 4 and Interrupts is the new concept todemonstrate for this lab.
The [LCD](https://github.com/jasonmossing15/LCD_Libraries) and [Button](https://github.com/jasonmossing15/Buttons) repositories used are shown in my github, or click on the links.

### Interrupts

There are two interrupts used: TimerA and P2.
TimerA is set to trigger every half a second by using the following code:
```
TACTL |= TASSEL1;
TACTL |= ID1|ID0;
```
This is done so when the ISR is called, a timer count increments and a count of 4 means that it has been 2 seconds. Once the count equals 4 then the user loses and the a display screen of "Game Over!" is shown. The only way out of this screen is by the use of buttons.


The buttons are controlled by the P2 interrupt. In this code there are four buttons, one for each direction, controlled by P.2, P.3, P.4, and P.5. Once a button is hit the edge select flips, waiting for the release, then debounces once released. The player then moves accordingly.

### A Functionality

In order to get A functionality, the random winnable generation of mines is needed. This was done by using the:
`prand();`
function given to us by the random number repo. Once creating a random number we needed to mod it and add it to 0x81 for the first line and 0xC0 for the second. You mod the number because there are 7 possible spots available on each line for the mines and this is the best way to generate a random location for each mine. After the generation you need to check to see if the game is winnable. This was done by using the function:
`isGameWinnable();`
This function checked the lower nibbles of the two mines and checked to see if they were within + or - 1. Being within this range means that there is no way around the mines, making the game unwinnable. 

## Documentation

I used the game shell given by Capt Branchflower and the random number generator repo. Feel free to use/work on this code!
