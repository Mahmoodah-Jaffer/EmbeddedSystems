//********************************************************************
//*                    EEE2046F C example                            *
//*==================================================================*
//* WRITTEN BY: Mahmoodah Bi Jaffer  	                 		                         *
//* DATE CREATED: 11 April 2018                                                 *
//* MODIFIED:                                                        *
//*==================================================================*
//* PROGRAMMED IN: Eclipse Neon 3 Service Release 1 (4.4.1)          *
//* DEV. BOARD:    UCT STM32 Development Board                       *
//* TARGET:	   STMicroelectronics STM32F051C6                        *
//*==================================================================*
//* DESCRIPTION:                                                     *
//*                                                                  *
//********************************************************************
// INCLUDE FILES
//====================================================================
#include "lcd_stm32f0.h"
#include "stm32f0xx.h"
#include "math.h"
//====================================================================
// SYMBOLIC CONSTANTS
//====================================================================
#define DELAY1 2000
#define DELAY2 1500
//====================================================================
// GLOBAL VARIABLES
//====================================================================
int step = 0;
int final = 128; //2^7=128
int lcd;
//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void init_GPIOB(void);
void Delay(void);
//====================================================================
// MAIN FUNCTION
//====================================================================
void main (void)
{
	init_GPIOB();
	init_LCD();							// Initialise lcd
	lcd_putstring("Mahmoodah Jaffer");	// Display string on line 1
	lcd_command(LINE_TWO);				// Move cursor to line 2
	lcd_putstring("JFFMAH001");	// Display string on line 2
	for(;;)
	{
		step =0;
		for(lcd=1;lcd<=final;lcd++){
			GPIOB->MODER|=0x00505555;
			lcd = pow(2,step);
			GPIOB->ODR=lcd;
			Delay();
			step++;
		}


	}									// Loop forever
}										// End of main

//====================================================================
// FUNCTION DEFINITIONS
//delay loop function
void Delay(void)
{
	for (int x=0; x<=DELAY1;x++)
		for (int y=0; y<=DELAY2;y++);
}
//====================================================================
void init_GPIOB(void)
{
	RCC  ->AHBENR |= 1<<18;
	GPIOB->MODER  |= 0x00505555;
	//GPIOB->ODR     = 0b0000010000001111;
}


//********************************************************************
// END OF PROGRAM
//********************************************************************
