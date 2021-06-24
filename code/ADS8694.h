#ifndef AD8694_H
#define AD8694_H
#include <USIC.h>
#include <Bits.h>

typedef	enum 
{
	AUTO_SEQ_EN = 1,
	FEATURE = 3,
	CH0_RANGE = 5,
	CH1_RANGE = 6,
	CH2_RANGE = 7,
	CH3_RANGE = 8,
	ALARM_OVERVIEW = 0x10,
	ALARM_TRIPPED = 0x11,
	ALARM_ACTIVE = 0x12,
	CH0_HYSTERESIS = 0x15,
	CH0_HIGH_M = 0x16,
	CH0_HIGH_L = 0x17,
	CH0_LOW_M = 0x18,
	CH0_LOW_L = 0x19,
	
	CH1_HYSTERESIS = 0x1A,
	CH1_HIGH_M = 0x1B,
	CH1_HIGH_L = 0x1C,
	CH1_LOW_M = 0x1D,
	CH1_LOW_L = 0x1E,
	
	CH2_HYSTERESIS = 0x1F,
	CH2_HIGH_M = 0x20,
	CH2_HIGH_L = 0x21,
	CH2_LOW_M = 0x22,
	CH2_LOW_L = 0x23,

	CH3_HYSTERESIS = 0x24,
	CH3_HIGH_M = 0x25,
	CH3_HIGH_L = 0x26,
	CH3_LOW_M = 0x27,
	CH3_LOW_L = 0x28,
	
	CMD_RD = 0x3F,
}  RegsADS8694;
	
typedef enum
{
	NOP = 0x0000,
	AUTO_CH = 0x0A00
}  Commandos;

typedef enum
{
	ADS_READY,
	ADS_IDDLE,
	ADS_WORK
} ADStatus;

typedef struct
{
	SPIDev* spiDv;
	unsigned long data[4];
	ADStatus stat;
} ADS8694;
//	unsigned char slN;
//	USIC_CH_TypeDef* spi;
	
// static bool WriteReg(SPIDev* chip, RegsAD8694 rg, unsigned long val);
// static bool WriteCmd(Commandos cmd, unsigned long &rxData);
//bool AD8694WriteCmd(SPIDev* chip, Commandos cmd);

unsigned char AD8694ReadReg(ADS8694*  chip, RegsADS8694 rg);
		
void AD8694Ini(ADS8694* chip);

void AD8694CnvStart(ADS8694* chip);

bool ADS8694GetVal(ADS8694* chip, int* data);

bool ADS8694ReadStart(ADS8694* chip, int* data);
 
#endif
