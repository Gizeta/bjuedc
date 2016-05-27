#include <msp430g2553.h>
#include "adc.h"
#include "clock.h"
#include "comp.h"
#include "timer.h"
#include "uart.h"
#include "util.h"

extern float Measure_Value;

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

    Comparator_Enable();
    TimerA0_Enable();
    TimerA1_Enable();
    UART_Enable();

    P1DIR |= BIT6; // for test use

    while (1)
    {
    	Display_Integer(freqValue * 2);
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
				P1OUT ^= BIT6;
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
			}
			break;
		case 4:
			break;
		case 10:
			break;
	}
}
