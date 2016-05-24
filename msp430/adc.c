#include <msp430g2553.h>
#include "adc.h"
#include "util.h"

void ADC_Init()
{
	ADC10CTL0  = SREF_1 + ADC10SHT_2 + REF2_5V + REFON + ADC10ON + ADC10IE;
	ADC10CTL1  = ADC_INCH + CONSEQ_0;
	ADC10AE0  |= ADC_IN;
	ADC_CHSEL |= ADC_IN;
}

void ADC_Enable()
{
	ADC10CTL0 |= ENC + ADC10SC;
}

void ADC_Disable()
{
	ADC10CTL0 &= ~ENC;
}

uint ADC_Get()
{
	return ADC10MEM;
}

void ADC_REF2_5V()
{
	ADC10CTL0 |= REF2_5V;
}

void ADC_REF1_5V()
{
	ADC10CTL0 &= ~REF2_5V;
}
