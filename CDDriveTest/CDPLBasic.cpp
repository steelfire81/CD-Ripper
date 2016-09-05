#include "CDPLBasic.h"

// Constructor
CDPLBasic::CDPLBasic()
{
	previousUpdate = 0;
}

// notifyEnd
void CDPLBasic::notifyEnd()
{
	previousUpdate = 100;
	printf("100%%\n");
}

// notifyProgress
void CDPLBasic::notifyProgress(double progress)
{
	progress *= 100;
	
	if (progress >= previousUpdate + CDPLB_INTERVAL)
	{
		previousUpdate = progress;
		printf("%d%%\n", previousUpdate);
	}
}

// notifyStart
void CDPLBasic::notifyStart()
{
	previousUpdate = 0;
	printf("0%%\n");
}
