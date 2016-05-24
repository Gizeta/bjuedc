#ifndef ADC_H_
#define ADC_H_

#include "util.h"

#define ADC_INCH  INCH_0
#define ADC_CHSEL P1SEL
#define ADC_IN    BIT0

void ADC_Init();
void ADC_Enable();
void ADC_Disable();
uint ADC_Get();
void ADC_REF2_5V();
void ADC_REF1_5V();

#endif
