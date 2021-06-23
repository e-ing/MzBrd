#include <BoardDev.h>
#include <Bits.h>

PoutPort* GetYellowLED(void)
{
	static bool isIni = false;
	static PoutPort ret;
	if(isIni == false)
	{
		isIni = true;
		ret.port = GPs[GPP2];
		ret.pin = 0;
		IniGPO(&ret);
	}
	return &ret;
}

PoutPort* GetBlueLED(void)
{
	static bool isIni = false;
	static PoutPort ret;
	if(isIni == false)
	{
		isIni = true;
		ret.port = GPs[GPP0];
		ret.pin = 4;
		IniGPO(&ret);
	}
	return &ret;
}