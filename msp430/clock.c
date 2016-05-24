#include <msp430g2553.h>
#include "clock.h"

void Calibration_Trap()
{
	if (CALBC1_1MHZ == 0xff || CALDCO_1MHZ == 0xff)
		while(1);
}

void Clock_1MHz()
{
	Calibration_Trap();

	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL  = CALDCO_1MHZ;
}

void Clock_8MHz()
{
	Calibration_Trap();

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL  = CALDCO_8MHZ;
}

void Clock_12MHz()
{
	Calibration_Trap();

	BCSCTL1 = CALBC1_12MHZ;
	DCOCTL  = CALDCO_12MHZ;
}

void Clock_16MHz()
{
	Calibration_Trap();

	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL  = CALDCO_16MHZ;
}
