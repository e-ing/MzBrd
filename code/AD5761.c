#include <AD5761.h>
#include <TheTime.h>
#include <GPIOs.h>
#include <BoardDev.h>

static const unsigned long WORD_L = 12;
static const unsigned long FRAME_L = 24;
static  int lastVal[8];

static bool WriteReg(USIC_CH_TypeDef* spi, unsigned char slN,  AD5761Rgs rgAdr, unsigned long val)
{
	//configure DAC CS
  //configure CS polarity
	//configure word and faramen lenght	
	bool ret = false;
	if(SPIdeviceConf(spi, FRAME_L, WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
	{
		unsigned short data[2] = {(unsigned short)(val >> 12) | (rgAdr << 4),
															(unsigned short)(val & 0xFFF)};
		USICRxFIFOClean(spi);
	//	GPToggle( ledY );													
		FastUSICTxw(spi, data, 2);
		ret = true;
	}
	return ret;
}

static bool PureWriteReg(USIC_CH_TypeDef* spi, unsigned char slN,  AD5761Rgs rgAdr, unsigned long val)
{
	//configure DAC CS
  //configure CS polarity
	//configure word and faramen lenght	
	bool ret = false;
	if(SPIdeviceConf(spi, FRAME_L, WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
	{
		unsigned short data[2] = {(unsigned short)(val >> 12) | (rgAdr << 4),
															(unsigned short)(val & 0xFFF)};
//		USICRxFIFOClean(spi);							
		FastUSICTxw(spi, data, 2);
		ret = true;
	}
	return ret;
}

static void FastPureWriteReg(USIC_CH_TypeDef* spi,  AD5761Rgs rgAdr, unsigned long val)
{
		unsigned short data[2] = {(unsigned short)(val >> 12) | (rgAdr << 4),
															(unsigned short)(val & 0xFFF)};
		FastUSICTxw(spi, data, 2);
}



void AD5761ReadReg (SPIDev* chip,  AD5761Rgs rgAdr, unsigned short* data)
{
		while ( WriteReg(chip->spi, chip->sSelNum, rgAdr, 0) == false)
		{//wait Tx Buffer Ready and send cmd;
		}

		FastPureWriteReg(chip->spi, NOP_RG, 0);
		while( IsTxBuffEmpty(chip->spi) == false)
		{//wait the ent of Tx Empty frame 
		}	
		
		Sleep(0.01);
		unsigned short tmpM = FIFORead(chip->spi);
		tmpM = FIFORead(chip->spi);
		tmpM = FIFORead(chip->spi);// & 0xfff;
		unsigned short tmpL = FIFORead(chip->spi); //& 0xfff;
		data[1] = tmpL | ( (tmpM & 0xf)  << 12);//16 LSb == reg data
		data[0] = tmpM >> 4;//8MSb reg adr + any other info

}


void AD5761ini(SPIDev* chip)
{
	chip->spi = usics[chip->usicN * USICS_NUM + chip->chan];
	SPIini(chip->usicN, chip->chan, WORD_L, FRAME_L, 100000);
	OffSlaveSel(chip->spi, chip->usicN);
	PoutPort* ledY =  GetYellowLED();
	PoutPort* ledB = GetBlueLED();
	GPToggle( ledB );
	while( WriteReg (chip->spi, chip->sSelNum, WR_CTL_RG,   0x3A2) == false)
	{
	}//+-10V operations, 5% overrange, 2 comliment code, to midscale after reset->
	while( IsTxBuffEmpty(chip->spi) == false)
	{//wait the ent of Tx Empty frame 
	}	
	Sleep(1);		
	USICRxFIFOClean(chip->spi);
}
		

int AD5761GetVal(SPIDev* chip)
{
	return lastVal[chip->sSelNum];
}

bool AD5761SetVal(SPIDev* chip, unsigned short val)
{
	bool ret =  WriteReg(chip->spi, chip->sSelNum, WR_AND_UPDATE_RG, val);
	lastVal[chip->sSelNum] = ret? val : lastVal[chip->sSelNum];
	return ret;
}

