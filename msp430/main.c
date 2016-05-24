#include <msp430g2553.h>
#include "adc.h"
#include "clock.h"
#include "timer.h"
#include "uart.h"
#include "util.h"

extern float Measure_Value;

int main()
{
    WDTCTL = WDTPW | WDTHOLD;

    Clock_1MHz();
    ADC_Init();
    Timer_Init();
    UART_Init();

    __enable_interrupt();

    Timer_Enable();
    UART_Enable();

    while (1)
    {
    	Display(Measure_Value);
    }
	return 0;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR()
{
	switch (TAIV)
	{
		case 2:
			break;
		case 4:
			break;
		case 10:
			Measure();
			break;
	}
}
