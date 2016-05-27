#include <msp430g2553.h>
#include "uart.h"

void UART_Init()
{
	P1SEL    |= BIT1 + BIT2;
	P1SEL2   |= BIT1 + BIT2;
	UCA0CTL0 &= ~UC7BIT;
	UCA0CTL1 |= UCSSEL_2 + UCSWRST;
	UCA0BR0   = 52;
	UCA0BR1   = 0;
	UCA0MCTL  = UCBRS0 + UCBRF1 + UCOS16;
}

void UART_Enable()
{
	UCA0CTL1 &= ~UCSWRST;
}

void UART_Disable()
{
	UCA0CTL1 |= UCSWRST;
}

void UART_Send(uchar data)
{
	while (!(UC0IFG & UCA0TXIFG));
	UCA0TXBUF = data;
}
