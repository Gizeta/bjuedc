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

void Display_Integer(int num)
{
	if (num >= 1000)
	{
		float number = num / 1000.0;
		Display_Float(number, 3);
		UART_Send(2 | (1 << 4));
	}
	else
	{
		char d0 = num % 10;
		char d1 = (num / 10) % 10;
		char d2 = num / 100;

		UART_Send(d2 | (4 << 4));
		__no_operation();
		UART_Send(d1 | (3 << 4));
		__no_operation();
		UART_Send(d0 | (2 << 4));
		__no_operation();
		UART_Send(1 | (1 << 4));
		__no_operation();

	}

}

void Display_Float(float num, char digit)
{
	char dx = 4, d = 0, flag = 0;
	int number1 = num;
	float number2 = num - number1;

	if (number1 > 99)
	{
		d = number1 / 100;
		UART_Send(d | (dx << 4));
		dx--;
		number1 %= 100;
		flag = 1;
	}
	if (flag == 1 && number1 < 10)
	{
		UART_Send(0 | (dx << 4));
		dx--;
	}
	if (number1 > 9)
	{
		d = number1 / 10;
		UART_Send(d | (dx << 4));
		dx--;
		number1 %= 10;
	}
	UART_Send(number1 | ((dx + 8) << 4));
	dx--;

	while (dx + digit > 4)
	{
		d = number2 * 10;
		UART_Send(d | (dx << 4));
		dx--;
		number2 = number2 * 10 - d;
	}
}
