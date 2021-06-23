#include <TheTime.h>

static volatile unsigned long msTicks;                      /* counts 1ms timeTicks       */

void SysTick_Handler()
{
  msTicks++;
}

void Delay (unsigned int ms)
{ 
  unsigned long curTicks = msTicks;
	unsigned long residue = 0xffffffff - curTicks;
	if(residue >= ms)
		while ((msTicks - curTicks) < ms)
		{
		}
	else
	{ 
		unsigned long tail = ms - residue - 1;
		while ( msTicks >= curTicks)
		{
		}
		while (msTicks < tail)
		{
		}
	}
}

void Sleep (double ms)
{
	double cycles =   ms * 10000.0;
	while	(cycles > 0)
		cycles -= 1;
}
