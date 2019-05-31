//********************************************************************
//*                    EEE2046F/EEE2050F C template                  *
//*==================================================================*
//* WRITTEN BY:    	                 		             *
//* DATE CREATED:                                                    *
//* MODIFIED:                                                        *
//*==================================================================*
//* PROGRAMMED IN: Eclipse Luna Service Release 1 (4.4.1)            *
//* TARGET:    PC or STM32F0?                                        *
//*==================================================================*
//* DESCRIPTION:                                                     *
//*                                                                  *
//********************************************************************
// INCLUDE FILES
//====================================================================
#include <stdio.h>
#include "lcd_stm32f0.h"
#include "stm32f0xx.h"
//====================================================================
// GLOBAL CONSTANTS
//====================================================================
#define DELAY1 1000 //Constant for Delay() function
#define DELAY2 3000 //Constant for Delay() function
//====================================================================
// GLOBAL VARIABLES
//====================================================================
int battery_voltage;
float k;
char buf[32];
//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void init_GPIO(void);
void init_ADC(void);
void check_battery(void);
void display(void);
void Delay(void);
//====================================================================
// MAIN FUNCTION
//====================================================================
int main (void)
{
	init_LCD();							// Initialise lcd
	lcd_putstring("Mahmoodah Jaffer");	// Display string on line 1
	lcd_command(LINE_TWO);				// Move cursor to line 2
	lcd_putstring("JFFMAH001");	// Display string on line 2
	for(;;)
	{
		init_GPIO();
		check_battery();
		display();
		Delay();
	}
}							// End of main

//====================================================================
// FUNCTION DEFINITIONS
//====================================================================
void init_GPIO(void)
{
		RCC  ->AHBENR |= 1<<18; // enable clock for port B
		GPIOB->MODER  |= 0x00505555; // Port B set to output mode
		GPIOB->ODR    = 0b0000000000000000; //Set all LEDs as off initally

		RCC  ->AHBENR |= 1<<17; // enable clock for port A
		GPIOA->MODER  |= 0x28000000; //switches set to input mode
		GPIOA->PUPDR |= 0b00100100000000000000000000010101;
		GPIO_IDR_5; //set mode of POT0 to analogue
}
void init_ADC(void)
{
	RCC  ->AHBENR |= RCC_AHBENR_GPIOAEN; //enable clock port A
	GPIOA->MODER  |= GPIO_MODER_MODER5;//Set PA5 to analogue mode

	RCC ->APB2ENR |= RCC_APB2ENR_ADCEN;// enable clock ADC

	ADC1 -> CR &=~ ADC_CR_ADSTART;// set ADSTART to 0

	ADC1 -> CFGR1 |= ADC_CFGR1_RES_1;// 8-bit resolution
	ADC1 -> CFGR1 &=~ ADC_CFGR1_ALIGN;//Right-aligned
	ADC1 -> CFGR1 |= ADC_CFGR1_CONT;//continuous mode

}
void check_battery(void)
{
	init_ADC();
	ADC1 -> CHSELR |= ADC_CHSELR_CHSEL5; //Channel 5 = PA5

	ADC1 -> CR |= ADC_CR_ADEN; // sets ADEN to "1" in ADC_CR Register

	while (ADC1 -> ISR & ADC_ISR_ADRDY ==0); //Loop is exited when ADRDY == 1

	ADC1 -> CR |= ADC_CR_ADSTART; // sets ADSTART to "1" in ADC_CR Register

	while(ADC1-> ISR & ADC_ISR_EOC == 0); //Loop is exited when EOC == 1

	battery_voltage = ADC1 -> DR; // writes value of battery_voltage to Data Register
// LED D9 is turned on when batter_voltage falls below 14V
    if (battery_voltage < 1190)
    {
    	GPIOB -> MODER |= 0x505555;
    	GPIOB -> ODR |= 0b110000000000;
    }
// LED D9 is turned off when battery_voltage is above 14V
    else
    {
    	GPIOB -> MODER |= 0x505555;
    	GPIOB -> ODR |= 0b000000000000;
    }

}
void display(void)
{
//Welcome message after SW0 is pressed
	if(!(GPIOA->IDR & GPIO_IDR_0))
	{
		init_LCD(); //Initialise LCD
		lcd_putstring("EE2046F Prac4B"); //Display on line 1
		lcd_command(LINE_TWO);// Move cursor to line 2
		lcd_putstring("Mahmoodah");//Display on line 2
	}
//Battery monitor message after SW1 is pressed
	else if(!(GPIOA->IDR & GPIO_IDR_1))
	{
		init_LCD();//Initialise LCD
		lcd_putstring("Battery Monitor");//Display on line 1
		lcd_command(LINE_TWO);// Move cursor to line 2
		lcd_putstring("Press SW2");//Display on line 2
	}

//Battery voltage displayed after SW2 is pressed
	else if (!(GPIOA->IDR & GPIO_IDR_2))
	{
		k= battery_voltage; //float variable k is set to the battery voltage
		sprintf(buf, "%02u.%03u V", (int) (k/10.5), (int) (((k/10.5)- (int) (k/10.5)) * 1000)); //Maps 147 to 14V
		check_battery(); //battery voltage is determined
		lcd_command(CLEAR); //Clears LCD screen
		lcd_putstring("Battery:");//Display on line 1
		lcd_command(LINE_TWO);//Move cursor to line 2
		lcd_putstring(buf);// Battery voltage is displayed on line 2
	}

}
//Delay function with for loop - to create delay
void Delay(void)
{
	for (int x=0; x<=DELAY1;x++)
		for (int y=0; y<=DELAY2;y++);
}
//********************************************************************
// END OF PROGRAM
//********************************************************************
