#include <msp430g2553.h>
#include "timer.h"

void TimerA0_Init()
{
	TA0CTL   = TASSEL_2 + MC_0 + TAIE;
	TA0CCR0  = 100; // 5kHz
	TA0CCR1  = 50;
	TA0CCTL1 = CCIE;
}

void TimerA1_Init()
{
	TA1CTL   = TASSEL_2 + MC_0 + TAIE;
	TA1CCR0  = 50000; // 10Hz
	TA1CCR1  = 25000;
	TA1CCTL1 = CCIE;
}

void TimerA0_Enable()
{
	TA0CTL |= MC_1 + TACLR;
}

void TimerA1_Enable()
{
	TA1CTL |= MC_1 + TACLR;
}

void TimerA0_Disable()
{
	TA0CTL &= MC_0;
}

void TimerA1_Disable()
{
	TA1CTL &= MC_0;
}
