#include <msp430g2553.h>
#include "adc.h"
#include "uart.h"
#include "util.h"

char REF_SEL = 0; // 1 => 1.5V, 2 => 2.5V
uint ADC_Value = 0;
float Measure_Value = 0;

void Measure()
{
	if (REF_SEL > 0) return;
	REF_SEL = 2;
	ADC_REF2_5V();
	ADC_Enable();
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR()
{
	ADC_Value = ADC_Get();
	if (REF_SEL == 2 && ADC_Value < 600)
	{
		REF_SEL = 1;
		ADC_Disable();
		ADC_REF1_5V();
		ADC_Enable();
	}
	else
	{
		Measure_Value = ADC_Value * SIZE_FACTOR * (REF_SEL == 2 ? 2.5 : 1.5) / 1023;
		ADC_Disable();
		REF_SEL = 0;
	}
}

void Display(float num)
{
	int number = num * 1000;
	char d3 = number % 10;
	char d2 = (number / 10) % 10;
	char d1 = (number / 100) % 10;
	char d0 = number / 1000;

	UART_Send(d3 | 0x80);
	__no_operation();
	//__delay_cycles(50000);
	UART_Send(d2 | 0x40);
	__no_operation();
	//__delay_cycles(50000);
	UART_Send(d1 | 0x20);
	__no_operation();
	//__delay_cycles(50000);
	UART_Send((d0 + 10) | 0x10);
	__no_operation();
	//__delay_cycles(50000);
}
