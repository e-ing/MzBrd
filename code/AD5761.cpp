#include <AD5761.h>
#include <TheTime.h>
//	unsigned char slN;
//	Abstract_oBit* pinLoad;
//	USIC_CH_TypeDef* spi;
bool AD5761::SetVal(int val)
{
	bool ret =  WriteReg(WR_AND_UPDATE_RG, val);
	lastVal = ret? val : lastVal;
	return ret;
}
	
bool AD5761::WriteReg(Regs rgAdr, unsigned long val)
{
	//configure DAC CS
  //configure CS polarity
	//configure word and faramen lenght	
	bool ret;
	if(SPIdeviceConf(spi, FRAME_L, WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
	{
		unsigned short data[2] = {(unsigned short)(val >> 12) | rgAdr << 4,
															(unsigned short)(val & 0xFFF)};
		USICRxFIFOClean(spi);
		FastUSICTxw(spi, data, 2);
		ret = true;
	}
	else
		ret = false;
	return ret;
}
	
bool AD5761::ReadReg(Regs rgAd, unsigned long &data)
{
	unsigned short rxData [2];
	while ( ! WriteReg(rgAd,  0))
	{//wait Tx Buffer Ready and send cmd;
	}
	Sleep(0.002);
	while ( ! WriteReg(NOP_RG, 0))
	{//wait Tx Buffer Ready and send empty frame;
	}
	while( ! IsTxBuffEmpty(spi))
	{//wait the ent of Tx Empty frame 
	}
	//Read RxFIFO
	unsigned int nWords = USICRxw(spi, rxData);
	bool ret = (nWords == 2)? true : false;
	data = rxData[0] & 0xfff;
	data <<= 12;
	data |= rxData[1] & 0xfff; 
	return ret;
}

AD5761::AD5761(unsigned char usicN, unsigned char chan, unsigned char sSelNum, AbstrBitOut &loadPin ):
slN(sSelNum),
pinLoad(loadPin)
{
	pinLoad = loadPin;
	spi = usics[usicN * USICS_NUM + chan];
	SPIini(usicN, chan, 12, 24 );
	OffSlaveSel(spi, slN);
	pinLoad = false;// sync Mode

	WriteReg (WR_CTL_RG, 0x3A9);//+-10V operations, 5% overrange, 2 comliment code, to midscale after reset.
}

AbstrIntOut&  AD5761::operator = (const char* cmd)
{
	return *this;
}




