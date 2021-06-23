#ifndef AD5761_H
#define AD5761_H
#include <USIC.h>
#include <Bits.h>

typedef enum 
	{
		NOP_RG = 0,
		WR_WO_UPDATE_RG = 1,
		UPDATE_RG = 2,
		WR_AND_UPDATE_RG = 3,
		WR_CTL_RG = 4,
		RD_INPUT_RG = 0xA,
		RD_OUT_RG = 0xB,
		RD_CTL_RG = 0xC
	} AD5761Rgs;

typedef struct
{
	unsigned char usicN;
	unsigned char chan;
	unsigned char sSelNum;
	USIC_CH_TypeDef* spi;
} ChipAD5761;

void ReadReg (ChipAD5761* chip,  AD5761Rgs rgAdr, unsigned short* data);

void AD5761ini(ChipAD5761* chip);

bool AD5761SetVal(ChipAD5761* chip, unsigned short val);

int AD5761GetVal(ChipAD5761* chip);



//class AD5761 : public AbstrIntOut
//{
//protected:
//	enum Regs
//	{
//		NOP_RG = 0,
//		WR_WO_UPDATE_RG = 1,
//		UPDATE_RG = 2,
//		WR_AND_UPDATE_RG = 3,
//		WR_CTL_RG = 4,
//		RD_INPUT_RG = 0xA,
//		RD_OUT_RG = 0xB,
//		RD_CTL_RG = 0xC
//	};

//	static const unsigned long WORD_L = 12;
//	static const unsigned long FRAME_L = 24;
//	unsigned char slN;
//	AbstrBitOut &pinLoad;
//	USIC_CH_TypeDef* spi;
//	
//	
//	virtual bool WriteReg(Regs rgAdr, unsigned long val);
//	virtual bool ReadReg(Regs rgAd, unsigned long &data);
//public:
//	AD5761(unsigned char usicN, unsigned char chan, unsigned char sSelNum, AbstrBitOut &loadPin );
//	virtual bool SetVal(int val);
//	virtual  AbstrIntOut&  operator =  (const char* cmd); 
//	virtual ~AD5761(){}
//};

#endif
