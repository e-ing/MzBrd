#include <Str.h>
#include <USIC.h>
#include <TheTime.h>
#include <stdio.h>

void Enter(void)
{
	Tx1(1, 1, 13);
	Tx1(1, 1, 10);
}


void SendStr(const char* str)
{
	if(str != 0)
		do
		{
			Tx1(1, 1, *str);
			Delay(10);
		}
		while(*++str);	
}



void Send(const char* str, unsigned long data, PrintMode mode)
{
	char strEx[32];
	SendStr(str);
	sprintf (strEx, (mode == HEX)? "%x" : "%d", (unsigned int) data);
	SendStr (strEx);
}

void SendLn(const char* str, unsigned long data, PrintMode mode)
{
	Send(str, data, mode);
	Enter();
}
