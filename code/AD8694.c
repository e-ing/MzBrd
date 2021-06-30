#include <ADS8694.h>
#include <TheTime.h>
#include <Str.h>
#include <BoardDev.h>

static const unsigned long ALARM_EN = B4;
static const unsigned long FULL_INFO = 3; //Bits 0-8 contain: channel adress, device address and inpet renge. SBAS686 p.54 table12
static const unsigned long WORD_L = 12;
static const unsigned long DT_WORD_L = 12;
//static const unsigned long CMD_WORD_L = 16;
static const unsigned long DATA_FRAME_L = 36;
static const unsigned long REG_FRAME_L = 24;


static void WriteCmd(USIC_CH_TypeDef* spi, Commandos cmd)
{
	unsigned short data[] = {cmd, 0, 0 };
	FastUSICTxw(spi, data, 3);
}

static void WriteReg(USIC_CH_TypeDef* spi, RegsADS8694 rg, unsigned short val)
{
	//USIC_CH_TypeDef* spi = chip->spiDv->spi;
//	while( !SPIdeviceConf(spi, WORD_L, REG_FRAME_L, chip->spiDv->sSelNum, CSPOL_LOW, MSB_FIRST) );
	unsigned short d0 = rg;
	d0 <<= 5;
	d0 |=  (1 << 4) | (val >> 4);
	val &= 0xf;
	val <<= 8;
	unsigned short data[2] = {d0, val};
	FastUSICTxw(spi, data, 2);	
}


unsigned char ADS8694ReadReg(ADS8694* chip, RegsADS8694 rg)
{
		USIC_CH_TypeDef* spi = chip->spiDv->spi;
		while( !IsTxBuffEmpty(spi) )
		{//wait the ent of Tx Empty frame 
		}	
		USICRxFIFOClean(spi);
		
		unsigned short d0 = rg;
		d0 <<= 5;
		unsigned short data[2] = {d0, 0};
		FastUSICTxw(spi, data, 2);		
		while( !IsTxBuffEmpty(spi) )
		{//wait the ent of Tx Empty frame 
		}	
		int tmp;
		Sleep(0.01);
		
		tmp = FIFORead(spi); //12 empty msb
		tmp = FIFORead(spi) & 0xff;
		return (unsigned char) tmp;
}
		
void ADS8694Ini(ADS8694* chip)
{
	USIC_CH_TypeDef* spi = usics[chip->spiDv->usicN * USICS_NUM + chip->spiDv->chan];
	chip->spiDv->spi = spi;
	//SPIini(chip->usicN, chip->chan, WORD_L, FRAME_L, 100000);
	SPIini(chip->spiDv->usicN, chip->spiDv->chan,  WORD_L, REG_FRAME_L, 100000);
	OffSlaveSel(chip->spiDv->spi, chip->spiDv->sSelNum);
	while( !SPIdeviceConf(spi,  REG_FRAME_L, WORD_L, chip->spiDv->sSelNum, CSPOL_LOW, MSB_FIRST) )
	{
	}
	USICRxFIFOClean(spi);
	PoutPort* blue = GetBlueLED();
//	while (true)
//	{
		Sleep(1);
		WriteReg (spi, AUTO_SEQ_EN, 0xf);
		Sleep(1);
		SendLn("A_S_EN0:" , FIFORead(spi), HEX);
		SendLn("A_S_EN1:" , FIFORead(spi), HEX);

		GPToggle(blue);
	//}
	WriteReg (spi, FEATURE, ALARM_EN | FULL_INFO);
	Sleep(1);
	SendLn("Feature0:" , FIFORead(spi), HEX);
	SendLn("Feature1:" , FIFORead(spi), HEX);
	
	Sleep(1);
	WriteReg (spi, PDW, 0xF0);
	Sleep(1);
	SendLn("PDW0:" ,  FIFORead(spi), HEX);
	SendLn("FPDW1:" , FIFORead(spi), HEX);

////	WriteReg (spi, CH0_RANGE, 0); 	// +-2.5*4.096V	
////	Sleep(1);
////	SendLn("CH0Rg0-0:" , FIFORead(spi), HEX);
////	SendLn("CH0Rg0-1:" , FIFORead(spi), HEX);
////	WriteReg (spi, CH1_RANGE, 0);		// +-2.5*4.096V
////	Sleep(1);
////	SendLn("CH0Rg1-0:" , FIFORead(spi), HEX);
////	SendLn("CH0Rg1-1:" , FIFORead(spi), HEX);
////	WriteReg (spi, CH2_RANGE, 0);		// +-2.5*4.096V
////	Sleep(1);
////	SendLn("CH0Rg2-0:" , FIFORead(spi), HEX);
////	SendLn("CH0Rg2-1:" , FIFORead(spi), HEX);
////	WriteReg (spi, CH3_RANGE, 0);		// +-2.5*4.096V
////	Sleep(1);
////	SendLn("CH0Rg0-0:" , FIFORead(spi), HEX);
////	SendLn("CH0Rg0-1:" , FIFORead(spi), HEX);
////	while( !IsTxBuffEmpty(spi) )
////	{//wait the ent of Tx Empty frame 
////	}	
	Sleep(1);
	USICRxFIFOClean(spi);
	chip->stat = ADS_IDDLE;
	SendLn("as=", ADS8694ReadReg(chip, AUTO_SEQ_EN), HEX);
	SendLn("pDw=", ADS8694ReadReg(chip, PDW), HEX);
	SendLn("ftch=", ADS8694ReadReg(chip, FEATURE), HEX);
	Delay(3000);
	SendStr("ini done");
}

void ADS8694CnvStart(ADS8694* chip)
{
	USIC_CH_TypeDef* spi = chip->spiDv->spi;
	while( !SPIdeviceConf(spi, DATA_FRAME_L, DT_WORD_L,  chip->spiDv->sSelNum, CSPOL_LOW, MSB_FIRST) );
	chip->stat = ADS_READY;
	USICRxFIFOClean(spi);	
	WriteCmd(spi, AUTO_CH);
//	for( int i = 0; i < 4; ++i)
//	{
//				Sleep(0.1);
//				WriteCmd(spi, CHAN_1 );
//				Sleep(0.1);
//				WriteCmd(spi, CHAN_2 );
//				Sleep(0.1);
//				WriteCmd(spi, CHAN_3 );
//				Sleep(0.1);
//				WriteCmd(spi, CHAN_0 );

//	}
	chip->stat = ADS_WORK;	
}

bool ADS8694GetVal(ADS8694* chip, int* data)
{
	bool ret;
	if(chip->stat == ADS_WORK)
	{
		USIC_CH_TypeDef* spi = chip->spiDv->spi;
		while( !IsTxBuffEmpty (spi) )
		{//wait the ent of Tx Empty frame 
		}	
		ret = true;
		int tmpM,  tmpL;
		Sleep(1);
		chip->stat = ADS_READY;
//		tmpL = FIFORead(spi);
//		tmpL = FIFORead(spi);
//		tmpL = FIFORead(spi);//cmd frame
		
		for (char adCh = 0; adCh < 3; ++adCh)
		{
//			tmpM = FIFORead(spi); //12 empty msb - e
//			tmpM = FIFORead(spi) & 0xff;
//			tmpM <<= 10;
//			tmpL = (FIFORead(spi) >> 2) & 0x3ff;
//			data[adCh] = tmpL | tmpM;
				data[adCh] = FIFORead(spi);
				Sleep(1);
		}		
		chip->stat = ADS_IDDLE;
	}
	else
		ret = false;
	return ret;
}

bool ADS8694ReadStart(ADS8694* chip, int* data)
{
	bool ret = ADS8694GetVal(chip, data);
	ADS8694CnvStart(chip);
	return ret;
}




bool ADS8694Ch0ReadStart(ADS8694* chip, int* data)
{
	bool ret;
	USIC_CH_TypeDef* spi = chip->spiDv->spi;
	if(chip->stat == ADS_WORK)
	{
		while( !IsTxBuffEmpty (spi) )
		{//wait the ent of Tx Empty frame 
		}	
		ret = true;
		int tmpM,  tmpL;
		Sleep(1);
		chip->stat = ADS_READY;
		for (char wrd = 0; wrd < 3; ++wrd)
		{
				data[wrd] = FIFORead(spi);
				Sleep(1);
		}		
		chip->stat = ADS_IDDLE;
	}
	else
		ret = false;
	while( !SPIdeviceConf(spi, DATA_FRAME_L, DT_WORD_L,  chip->spiDv->sSelNum, CSPOL_LOW, MSB_FIRST) );
	chip->stat = ADS_READY;
	USICRxFIFOClean(spi);	
	WriteCmd(spi, CHAN_0);

	chip->stat = ADS_WORK;	
	return ret;
}

//ADS8694::ADS8694(unsigned char usicN, unsigned char chan, unsigned char sSelNum):	
//I_ADC<vector<int>,int>(), 	
//slN(sSelNum)
//{
//	data.resize(4, 0);
//	spi = usics[usicN * USICS_NUM + chan];
//	SPIini(usicN, chan, 17, 34 );
//	OffSlaveSel(spi, slN);
//	while(! WriteReg (AUTO_SEQ_EN, 0xf) );//+-10V operations, 5% overrange, 2 comliment code, to midscale after reset.
//	while(! WriteReg (FEATURE, ALARM_EN | FULL_INFO));
//	while(! WriteReg (CH0_RANGE, 0)); 	// +-2.5*4.096V
//	while(! WriteReg (CH1_RANGE, 0));		// +-2.5*4.096V
//	while(! WriteReg (CH2_RANGE, 0));		// +-2.5*4.096V
//	while(! WriteReg (CH3_RANGE, 0));		// +-2.5*4.096V
//	USICRxFIFOClean(spi);
//}

//void CnvStart()
//{
//	vector <int>::iterator it = data.begin();
//	while (! WriteCmd(AUTO_CH, it));
//	for( ; it != data.end(); ++it)
//		while(!WriteCmd(NOP, it));
//	ready =  DONE;
//}

//bool ADS8694::WriteReg(Regs rg, unsigned long val )
//{
//	bool ret;
//	if(SPIdeviceConf(spi, PR_WORD_L << 1, PR_WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
//	{
//		unsigned short tmp = rg;
//		unsigned short data[2] = {tmp << 5 | B4 | (unsigned short) val >> 4,
//															(val & 0xf) << 8	} ;
//		USICRxFIFOClean(spi);
//		FastUSICTxw(spi, data, 2);
//		ret = true;
//	}
//	else
//		ret = false;
//	return ret;
//}

//bool ADS8694::WriteCmd(Commandos cmd, vector<int>::iterator it)
//{
//	bool ret;
//	if(SPIdeviceConf(spi, CMD_FRAME_L, CMD_WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
//	{
//		unsigned short data[3] = {cmd, 0, 0};
//		USICRxFIFOClean(spi);
//		FastUSICTxw(spi, data, 2);
//		while( ! IsTxBuffEmpty(spi))
//		{//wait the ent of Tx Empty frame 
//		}
//		//Read RxFIFO
//		unsigned short rxW[3];
//		unsigned int nWords = USICRxw(spi, rxW);
//		*it = rxW[1];
//		*it <<= 2;
//		*it |= rxW[2] >> 14;
//		ret = (nWords == 3)? true : false;
//	}
//	else
//		ret = false;
//	return ret;
//}

//bool ADS8694::ReadReg(Regs rg, unsigned char &data)
//{
//	bool ret;
//	if(SPIdeviceConf(spi, PR_WORD_L << 1, PR_WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
//	{
//		unsigned short tmp = rg;
//		unsigned short txD[2] = { tmp << 5 , 0};
//		FastUSICTxw(spi, txD, 2);
//		while( ! IsTxBuffEmpty(spi))
//		{//wait the ent of Tx Empty frame 
//		}
//		unsigned short rxD [2];
//		unsigned int nWords = USICRxw(spi, rxD);
//		bool ret = (nWords == 2)? true : false;
//		rxD[0] = 1;
//		data = (unsigned char) ( rxD [1] & 0xff);
//	}
//	else
//		ret = false;
//	return ret;
//}

