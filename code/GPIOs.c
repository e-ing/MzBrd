#include <GPIOs.h>

volatile   GPIOctl* GPs [] = {(volatile  GPIOctl*) PORT0_BASE, (volatile   GPIOctl*) PORT1_BASE, (volatile   GPIOctl*) PORT2_BASE, (volatile GPIOctl*) PORT3_BASE,
															 (volatile GPIOctl*) PORT4_BASE, (volatile  GPIOctl*) PORT5_BASE, (volatile  GPIOctl*) PORT14_BASE, (volatile   GPIOctl*) PORT15_BASE};
///////////========INPUT===========
static void IniGPi(PinPort* pip)
{
	pip->maskIn = 1 << pip->pin;
	pip->port->IOCR[pip->pin >> 2] &= ~(0xf8 << (8 * (pip->pin & 3)));
}
															 
bool GetState(PinPort* pip)
{
	return pip->port->IN & (1 << pip->pin);
}

void GPinPullH(PinPort* pip)
{
	IniGPi(pip);
	pip->port->IOCR[pip->pin >> 2] |= 0x10 << (8 * (pip->pin & 3));	
}

void GPinPullD(PinPort* pip)
{
	IniGPi(pip);
	pip->port->IOCR[pip->pin >> 2] |= 8 << (8 * (pip->pin & 3));
}	

void  GPinPushPull(PinPort* pip)
{
	IniGPi(pip);
	pip->port->IOCR[pip->pin >> 2] &= ~(0xf8 << (8 * (pip->pin & 3)));
}															 
															 
															 
//////////=========OUT=============
void IniGPO (PoutPort* pop) 
{
	pop->port->IOCR[pop->pin >> 2] &= ~(0xf8 << (8 * (pop->pin & 3)));
	pop->port->IOCR[pop->pin >> 2] |= 0x80 << (8 * (pop->pin & 3));
	pop->maskOn = 1 << pop->pin;
	pop->maskOff = 1 << (pop->pin+16);
	pop->maskTogg = pop->maskOn | pop->maskOff;	
}

void GPToggle (PoutPort* pop)
{
	pop->port->OMR = pop->maskTogg;
}

void SetAltFn (PoutPort* pop, unsigned  long funcN)
{
	pop->port->IOCR[pop->pin >> 2] |= (funcN << (8 * (pop->pin & 3) + 3));
}


void GPSetVal (PoutPort* pop, bool val)
{
	pop->port->OMR = val? pop->maskOn : pop->maskOff;
}

void GPtoH (PoutPort* pop)
{
	pop->port->OMR =  pop->maskOn;
}


void GPtoL (PoutPort* pop)
{
	pop->port->OMR =  pop->maskOff;
}
