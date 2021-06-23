#include <GPIOs.h>

volatile   GPIOctl* GPs [] = {(volatile GPIOctl*) PORT0_BASE, (volatile  GPIOctl*) PORT1_BASE, (volatile  GPIOctl*) PORT2_BASE, (volatile  GPIOctl*) PORT3_BASE,
															 (volatile  GPIOctl*) PORT4_BASE, (volatile  GPIOctl*) PORT5_BASE, (volatile GPIOctl*) PORT14_BASE, (volatile  GPIOctl*) PORT15_BASE};


void IniGPO (volatile GPIOctl* port, unsigned char pin) 
{
	port->IOCR[pin >> 2] &= ~(0xf8 << (8 * (pin & 3)));
	port->IOCR[pin >> 2] |= 0x80 << (8 * (pin & 3));
}

void GPToggle (volatile GPIOctl* port, unsigned char pin)
{
	unsigned long pinMaskOn = 1 << pin;
	unsigned long  pinMaskOff = 1 << (pin+16);
	unsigned long pinMaskTogg = pinMaskOn | pinMaskOff;
	port->OMR = pinMaskTogg;
}

void SetAltFn (volatile GPIOctl* port, unsigned char pin, unsigned  long funcN)
{
	port->IOCR[pin >> 2] |= (funcN << (8 * (pin & 3) + 3));
}
