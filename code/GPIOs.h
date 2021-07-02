#ifndef GPIOS_H
#define GPIOS_H

#include  <XMC4300.h>
#include <Bits.h>
//#define bool char
//#define true 1
//#define	false 0



enum GPIOsPs
{
	GPP0,
	GPP1,
	GPP2,
	GPP3,
	GPP4,
	GPP5,
	GPP14,
	GPP15,
	GPPNUM
};

typedef struct 
{
	unsigned long  OUT;                               // (@ 0xBASEX000) Port Output Register                  
  unsigned long  OMR;                               // (@ 0xBASEX004) Port Output Modification Register
  unsigned long  RESERVED[2];
  unsigned long  IOCR[4]; 
//  unsigned long  IOCR0; 															// (@ 0xBASEX010) Port Input/Output Control Register 0
//  unsigned long  IOCR4;                             // (@ 0xBASEX014) Port Input/Output Control Register 4
//  unsigned long  IOCR8;                             // (@ 0xBASEX018) Port Input/Output Control Register 8 
//  unsigned long  IOCR12;                            // (@ 0xBASEX01C) Port Input/Output Control Register 12 
  unsigned long  RESERVED1;
  unsigned long  IN;                                // (@ 0xBASEX024) Port Input Register 
  unsigned long  RESERVED2[6];
  unsigned long  PDR0;                              // (@ 0xBASEX040) Port  Pad Driver Mode 0 Register  
  unsigned long  PDR1;                              // (@ 0xBASEX044) Port  Pad Driver Mode 1 Register 
  unsigned long  RESERVED3[6];
  unsigned long  PDISC;                             // (@ 0xBASEX060) Port  Pin Function Decision Control Register 
  unsigned long  RESERVED4[3];
  unsigned long  PPS;                               // (@ 0xBASEX070) Port  Pin Power Save Register 
  unsigned long  HWSEL;                             // (@ 0xBASEX074) Port  Pin Hardware Select Register
} GPIOctl;

typedef struct
{
	volatile GPIOctl* port;
	unsigned char pin;
	unsigned long maskIn;
} PinPort;

typedef struct
{
	volatile GPIOctl* port;
	unsigned char pin;
	unsigned long maskOn;   // = 1 << pin;
	unsigned long  maskOff; // = 1 << (pin+16);
	unsigned long maskTogg; //=(1 << pin) | (1 << (pin+16));
} PoutPort;

extern volatile GPIOctl* GPs [];

bool GetState(PinPort* pip);

void GPinPullH(PinPort* pip);

void GPinPullD(PinPort* pip);

void GPinPushPull(PinPort* pip);

//void SetSpeed(PinPort pip, unsigned int speed);

void IniGPO (PoutPort* pop);

void GPToggle (PoutPort* pop);

void GPSetVal (PoutPort* pop, bool val);

void GPtoH (PoutPort* pop);

void GPtoL (PoutPort* pop);

void SetAltFn (PoutPort* pop, unsigned  long funcN);

#endif
