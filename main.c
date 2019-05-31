//********************************************************************
//*                    EEE2046F C example                            *
//*==================================================================*
//* WRITTEN BY:    	                 		                         *
//* DATE CREATED:                                                    *
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
//====================================================================
// SYMBOLIC CONSTANTS
//====================================================================
#define DELAY1 1000 //Constant for Delay() function
#define DELAY2 3000 //Constant for Delay() function
//====================================================================
// GLOBAL VARIABLES
//====================================================================
char value;
char value1; //The incremented value
char value2; //The decremented value
int start=0;// Counter begins counting at zero
int stop = 255; //Maximum number counter can count to
//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void InitPorts(void);
char CountUp(char value); //increments value and returns incremented value
char CountDown(char value); //decrements value and returns decremented value
void Delay(void); //Declare Delay() function
//====================================================================
// MAIN FUNCTION
//====================================================================
void main (void)
{
	init_LCD();							// Initialise lcd
	lcd_putstring("Mahmoodah Jaffer");	// Display string on line 1
	lcd_command(LINE_TWO);				// Move cursor to line 2
	lcd_putstring("JFFMAH001");	// Display string on line 2
	for(;;)
	{
//represents postion not the value
		InitPorts();
		/* When SW0 is pressed for the first time the counter will start to count up from 0 up to 255 (it will not reset) if SW1 or SW2 is not pressed.
		 * When SW1 is pressed then the counter will continue to count up and once it reaches 255 it will reset to 0 and continue counting up.
		 * When SW2 is pressed then the counter will count down and once it reaches 0 it will reset to 255 and continue to count down.
		 * If SW0 is pressed while the counter is counting, the counter will reset to 0 and begin counting up from 0.
		 */
		while((!((GPIOA->IDR & 0b1) != 0) || start!=0 ) && ((GPIOA->IDR & 0b10 )!= 0) && ((GPIOA->IDR & 0b100) != 0)) //it'll remain false until SW0 is pressed
		{

			CountUp(start);
			Delay();//Delay function used to create a 1 second delay between increments
			start = CountUp(start);
			if (start==stop) //reset to 0 once it reaches 255
			{
				start=0;
			}

	    }
		if(!((GPIOA->IDR & 0b10) != 0))//When SW1 is pressed the counter will count up
		{
			while((((GPIOA->IDR & 0b1) != 0) ) && ((GPIOA->IDR & 0b100) != 0))
			{

				CountUp(start);
				Delay();
				start = CountUp(start);
				if (start==stop) // reset to 0 once it reaches 255 and carries on counting
				{
					start=0;
				}

		    }
		}
		if(!((GPIOA->IDR & 0b1) != 0))
		{
			start=0;
		}

		else if(!((GPIOA->IDR & 0b100) != 0))// When SW2 is pressed it will count down
		{
			while((((GPIOA->IDR & 0b1) != 0) ) && ((GPIOA->IDR & 0b10) != 0))
			{

				CountDown(start);
				Delay();
				start = CountDown(start);
				if (start==0)//Once the counter reaches 0 it will reset to 255 and carry on counting down
				{
					start=stop;
				}

		    }
		}
		if(!((GPIOA->IDR & 0b1) != 0))
		{
			start=0;
		}


	}									// Loop forever
}										// End of main

//====================================================================
// FUNCTION DEFINITIONS
//====================================================================
void InitPorts(void)
{

	RCC  ->AHBENR |= 1<<18;
	GPIOB->MODER  |= 0x00505555;
	GPIOB->ODR    = 0b0000000000000000;

	RCC  ->AHBENR |= 1<<17; //Because its GPIOA not GPIOB
	GPIOA->MODER  |= 0x28000000;
	//IDR is unsettable
	GPIOA->PUPDR |= 0b00100100000000000000000000010101;

}
//function which increments the value taken in and returns the incremented value
char CountUp(char value)
{

	GPIOB->MODER  |= 0x00505555;
	GPIOB->ODR    = value;
	value1= value +1;
	return value1;

}
//function which decrements the value taken in and returns the decremented value
char CountDown(char value)
{

	GPIOB->MODER  |= 0x00505555;
	GPIOB->ODR    = value;
	value2= value -1;
	return value2;

}
//Delay function with for loop
void Delay(void)
{
	for (int x=0; x<=DELAY1;x++)
		for (int y=0; y<=DELAY2;y++);
}

//********************************************************************
// END OF PROGRAM
//********************************************************************
