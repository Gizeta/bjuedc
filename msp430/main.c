/**
 * P1.0 <- ADC
 * P1.1 <- UART(RXD)
 * P1.2 <- UART(TXD)
 * P1.3 <- Comparator
 * P1.4 <- Function Selector1
 * P1.5 <- Function Selector2
 */

#include <msp430g2553.h>
#include "adc.h"
#include "clock.h"
#include "comp.h"
#include "timer.h"
#include "uart.h"
#include "util.h"

#define DC_VOLT_SIZE_FACTOR 1
#define DC_CUR_SIZE_FACTOR  1
#define AC_VOLT_SIZE_FACTOR 0.707

extern float Measure_Value;
extern float Measure_MaxValue;
float Display_Value = 0;

uint displayCount = 0;
uint timer0Count = 0;
uint timer1Count = 0;

char cmpFlag = 0;
uint freqCount = 0;
uint freqValue = 0;

int main()
{
    WDTCTL = WDTPW | WDTHOLD;

    Clock_8MHz();
    ADC_Init();
    Comparator_Init();
    TimerA0_Init();
    TimerA1_Init();
    UART_Init();

    __enable_interrupt();

    ADC_Enable();
    Comparator_Enable();
    TimerA0_Enable();
    TimerA1_Enable();
    UART_Enable();

    P1DIR &= ~(BIT4 + BIT5);
    P1OUT |= BIT4 + BIT5;
    P1REN |= BIT4 + BIT5;

    while (1)
    {
    	if ((P1IN & BIT4) == 0)
    	{
    		if ((P1IN & BIT5) == 0)
    		{
    			Display_Float(Display_Value * DC_VOLT_SIZE_FACTOR, 4);
    		}
    		else
    		{
    			Display_Float(Display_Value * DC_CUR_SIZE_FACTOR, 4);
    		}
    	}
    	else
    	{
    		if ((P1IN & BIT5) == 0)
    		{
    			Display_Float(Measure_MaxValue * AC_VOLT_SIZE_FACTOR, 4);
    		}
    		else
    		{
    			Display_Integer(freqValue * 2);
    		}
    	}
    }
    return 0;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR()
{
	switch (TA0IV)
	{
		case 2:
			timer0Count = (timer0Count + 1) % 8;
			if (timer0Count == 0)
			{
				if (CAOUT & CACTL2)
				{
					if (cmpFlag == 0)
					{
						cmpFlag = 1;
					}
				}
				else
				{
					if (cmpFlag == 1)
					{
						freqCount++;
						cmpFlag = 0;
					}
				}
			}
			break;
		case 4:
			break;
		case 10:
			break;
	}
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR()
{
	switch (TA1IV)
	{
		case 2:
			timer1Count = (timer1Count + 1) % 80;
			if (timer1Count == 0)
			{
				freqValue = freqCount;
				freqCount = 0;
				Measure_MaxValue = 0;
			}
			displayCount = (displayCount + 1) % 30;
			if (displayCount == 0)
			{
				Display_Value = Measure_Value;
			}
			Measure();
			break;
		case 4:
			break;
		case 10:
			break;
	}
}
