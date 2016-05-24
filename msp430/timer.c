#include <msp430g2553.h>
#include "timer.h"

void Timer_Init()
{
	TACTL = TASSEL_2 + ID_1 + TACLR + MC_0 + TAIE;
}

void Timer_Enable()
{
	TACTL |= MC_2 + TACLR;
}

void Timer_Disable()
{
	TACTL &= MC_0;
}
