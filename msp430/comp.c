#include <msp430g2553.h>
#include "comp.h"

void Comparator_Init()
{
	CACTL1 = CARSEL + CAREF_2;
	CACTL2 = P2CA2 + P2CA1;
	CAPD   = CAPD3;
}

void Comparator_Enable()
{
	CACTL1 |= CAON;
}

void Comparator_Disable()
{
	CACTL1 &= ~CAON;
}
