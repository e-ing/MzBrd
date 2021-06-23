#include <AD8694.h>

static const unsigned long ALARM_EN = B4;
static const unsigned long FULL_INFO = 3; //Bits 0-8 contain: channel adress, device address and inpet renge. SBAS686 p.54 table12
static const unsigned long PR_WORD_L = 12;
static const unsigned long CMD_WORD_L = 16;
static const unsigned long CMD_FRAME_L = 34;


AD8694::AD8694(unsigned char usicN, unsigned char chan, unsigned char sSelNum):	
I_ADC<vector<int>,int>(), 	
slN(sSelNum)
{
	data.resize(4, 0);
	spi = usics[usicN * USICS_NUM + chan];
	SPIini(usicN, chan, 17, 34 );
	OffSlaveSel(spi, slN);
	while(! WriteReg (AUTO_SEQ_EN, 0xf) );//+-10V operations, 5% overrange, 2 comliment code, to midscale after reset.
	while(! WriteReg (FEATURE, ALARM_EN | FULL_INFO));
	while(! WriteReg (CH0_RANGE, 0)); 	// +-2.5*4.096V
	while(! WriteReg (CH1_RANGE, 0));		// +-2.5*4.096V
	while(! WriteReg (CH2_RANGE, 0));		// +-2.5*4.096V
	while(! WriteReg (CH3_RANGE, 0));		// +-2.5*4.096V
	USICRxFIFOClean(spi);
}

void AD8694::CnvStart()
{
	vector <int>::iterator it = data.begin();
	while (! WriteCmd(AUTO_CH, it));
	for( ; it != data.end(); ++it)
		while(!WriteCmd(NOP, it));
	ready =  DONE;
}

bool AD8694::WriteReg(Regs rg, unsigned long val )
{
	bool ret;
	if(SPIdeviceConf(spi, PR_WORD_L << 1, PR_WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
	{
		unsigned short tmp = rg;
		unsigned short data[2] = {tmp << 5 | B4 | (unsigned short) val >> 4,
															(val & 0xf) << 8	} ;
		USICRxFIFOClean(spi);
		FastUSICTxw(spi, data, 2);
		ret = true;
	}
	else
		ret = false;
	return ret;
}

bool AD8694::WriteCmd(Commandos cmd, vector<int>::iterator it)
{
	bool ret;
	if(SPIdeviceConf(spi, CMD_FRAME_L, CMD_WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
	{
		unsigned short data[3] = {cmd, 0, 0};
		USICRxFIFOClean(spi);
		FastUSICTxw(spi, data, 2);
		while( ! IsTxBuffEmpty(spi))
		{//wait the ent of Tx Empty frame 
		}
		//Read RxFIFO
		unsigned short rxW[3];
		unsigned int nWords = USICRxw(spi, rxW);
		*it = rxW[1];
		*it <<= 2;
		*it |= rxW[2] >> 14;
		ret = (nWords == 3)? true : false;
	}
	else
		ret = false;
	return ret;
}

bool AD8694::ReadReg(Regs rg, unsigned char &data)
{
	bool ret;
	if(SPIdeviceConf(spi, PR_WORD_L << 1, PR_WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
	{
		unsigned short tmp = rg;
		unsigned short txD[2] = { tmp << 5 , 0};
		FastUSICTxw(spi, txD, 2);
		while( ! IsTxBuffEmpty(spi))
		{//wait the ent of Tx Empty frame 
		}
		unsigned short rxD [2];
		unsigned int nWords = USICRxw(spi, rxD);
		bool ret = (nWords == 2)? true : false;
		rxD[0] = 1;
		data = (unsigned char) ( rxD [1] & 0xff);
	}
	else
		ret = false;
	return ret;
}

