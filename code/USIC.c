#include <USIC.h>
//#include <GPIO.h>
#include <Bits.h>
#include <TheTime.h>
#define bool char
#define true 1
#define false 0	
const unsigned char USICS_NUM = 2;
//BRG
const unsigned long PASSIV_CLK_HIGH = 30 << 1;
//SCTR
//const unsigned long WORD_LENGTH = 16;
//const unsigned long FRAME_LENGTH = 1;
const unsigned long SHIFT_CLK_ACTIV_LEVEL = 1 << 8;
const unsigned long MS_BIT_FIRST = 1;
const unsigned long LS_BIT_FIRST = 0;
const unsigned long PASSIV_DATA_HIGH_LOW = 0;
const unsigned long  PASSIV_DATA_HIGH= 1 << 1;
//TCSR
const unsigned long SINGLE_SHOT_MODE = 1 << 8;
const unsigned long TX_START = 1 << 10;;
//PCR SPI
const unsigned long GENERATE_SLAVE_SEL = 1;
const unsigned long DIRECT_SLAVE_SEL = 2;
const unsigned long ACTIV_LOW_SLAVE_SEL = 4;
//PCR UART
const unsigned long SMPL_IS_3 = 1;
const unsigned long SMPL_POINT_IS_8 = 8 << 8;
const unsigned long RX_STAT_EN = 1 << 16;
const unsigned long TX_STAT_EN = 1 << 17;
//CCR
const unsigned long NO_PARITY = 0;
const unsigned long SPI_MODE = 1;
const unsigned long UART_MODE = 2;
//FIFO
const unsigned long RX_FIFO_SZ = 32;
const unsigned long TX_FIFO_SZ = 32;
const unsigned long FIFO_SZ_2 = 1 << 24;
const unsigned long FIFO_SZ_4 = 2 << 24;
const unsigned long FIFO_SZ_8 = 3 << 24;
const unsigned long FIFO_SZ_16 = 4 << 24;
const unsigned long FIFO_SZ_32 = 5 << 24;
const unsigned long FIFO_SZ_64 = 6 << 24;
const unsigned long TX_FIFO_LIMIT = 1 << 8;
const unsigned long RX_FIFO_LIMIT = 30 << 8;
const unsigned long FIFO_TRIG_EN = 1 << 15;
const unsigned long FIFO_TRIG_MODE = 1 << 14;
const unsigned long CLR_STD_TX_BF_EV = 1 << 8;
const unsigned long STD_TX_BF_EV_TR = 1 << 14;
const unsigned long RX_DATA_READY = 1 << 14 | 1 << 13;

const unsigned long CS_CODE[] = {1 << 16, B17, B18, B19, B20, B21, B22, B23};
USIC_CH_TypeDef* usics[] = {USIC0_CH0, USIC0_CH1, USIC1_CH0, USIC1_CH1}; 
static bool isUsicIni[4] = {false, false, false, false}; 

void DisableAllSlavesSel(USIC_CH_TypeDef* s)
{
	s->PCR &= 0xFF00FFFF;
}

void SlaveSelMask(USIC_CH_TypeDef* s, unsigned char mask)//mask 0 - 0xff b0 - slave0, b1 - slave1.. 1 - active, 0 - off.
{
	s->PCR = mask << 16;
}

void ActivateSlaveSel(USIC_CH_TypeDef* s, unsigned char slaveNum)// num = 0-7;
{
	unsigned long mask = 1 << (slaveNum + 16);
	s->PCR |= mask;
}

void OffSlaveSel(USIC_CH_TypeDef* s, unsigned char slaveNum)// num = 0-7;
{
	unsigned long mask = 1 << (slaveNum + 16);
	s->PCR &= ~mask;
}



#include <stdlib.h>

static void Reset(USIC_CH_TypeDef* usic, unsigned char usicN)
{
		if(usicN == 0)
	{
		*(unsigned long *)&(SCU_RESET->PRCLR0) = 0x800;
    while (  *(unsigned long *)&(SCU_RESET->PRSTAT0) & 0x800    )///XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_USIC1));//XMC_SCU_PERIPHERAL_RESET_USIC1
		{
		}	
	}
	else if(usicN == 1)
	{
  *(unsigned long *)(&(SCU_RESET->PRCLR0) + 3 ) = 0x80;
	

    while (  *(unsigned long *)(&(SCU_RESET->PRSTAT0) + 3) & 0x80    )///XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_USIC1));//XMC_SCU_PERIPHERAL_RESET_USIC1
		{
		}
	}
	usic->KSCFG = 3; // Module enable & bit protection
	while ((usic->KSCFG & 1) == 0)//USIC_CH_KSCFG_MODEN_Msk) == 0U)
	{
	}
	//==Clock==
	usic->CCR &= 0xFFFFFFFF00;// Set USIC channel in IDLE mode
}

static void SetBaudRate(USIC_CH_TypeDef* usic, int bRate)
{
	static const unsigned int OVER_SMP = 16;
	unsigned int  prphClk = (SystemCoreClock >>
									 ((SCU_CLK->PBCLKCR & SCU_CLK_PBCLKCR_PBDIV_Msk) >> SCU_CLK_PBCLKCR_PBDIV_Pos)); 
	signed int bRateUpp, bRateLo;
	unsigned int brgClk = bRate * OVER_SMP, pdiv = 1,  divStp;
	if (prphClk > brgClk)
	{
		divStp = prphClk / brgClk; // integer division gets truncated
		while (divStp >= 1023)
		{
			brgClk = bRate * OVER_SMP * ++pdiv;
			divStp = prphClk / brgClk; // integer division gets truncated
		}
		bRateUpp = prphClk / (divStp * OVER_SMP * pdiv);
		bRateLo = prphClk / ((divStp + 1) * OVER_SMP * pdiv);
// choose better approximation if the peripheral frequency is not a multiple of the baudbRate
		if (abs (bRate - bRateLo) < abs(bRate - bRateUpp))
			 ++divStp;
		divStp = 1024 - divStp;
		usic->FDR = (1 << 14) | divStp;															//XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_NORMAL |	//(divStp //<< USIC_CH_FDR_STEP_Pos == 0);
		
	  usic->BRG &= ~( 0x7C00						 |//~USIC_CH_BRG_DCTQ_Msk
															0x3ff0000          | // USIC_CH_BRG_PDIV_Msk |
													    0x300	             | // USIC_CH_BRG_PCTQ_Msk |
													    0x10 );               //USIC_CH_BRG_PPPEN_Msk
		
		usic->BRG |=  ((OVER_SMP - 1) << 10) |	// USIC_CH_BRG_DCTQ_Pos)
		   								((pdiv -1) <<  16);    //USIC_CH_BRG_PDIV_Pos)							
	}
}


void SPIini(unsigned char usicN, unsigned char chan, unsigned long wLen, unsigned long frLen, int rate)
{	
	unsigned char num = usicN * USICS_NUM + chan;
	if (isUsicIni[num] != false)
		return;
	
	USIC_CH_TypeDef* spi = usics[num];	
	Reset(spi, usicN);
	SetBaudRate(spi, rate);

	spi->PCR_SSCMode = GENERATE_SLAVE_SEL | DIRECT_SLAVE_SEL | ACTIV_LOW_SLAVE_SEL;
	spi->SCTR = ((wLen - 1) << 24) | //word lenght
								(frLen - 1) << 16  |// fr.Lenght
                SHIFT_CLK_ACTIV_LEVEL  | PASSIV_DATA_HIGH;
	
	spi->TCSR = (1 << 10) | 0x100;
	spi->PSCR = 0xFFFFFFFF;
	spi->CCR = NO_PARITY;//_c_onfig.parity_mode;
	spi->TBCTR &= ~( 0x3F |  0x3F00   | 0x7000000);
  spi->TBCTR |=    16   | (1 << 8)  | 5 << 24 ;	// dataPtr == 16, limit==1, fifoSz == 32byte,
	
	spi->RBCTR = 0;									
	static const unsigned long RX_LIM = 30;
	static const unsigned long RX_D_PTR = 0;
	static const unsigned long RX_BUFF_SZ16 = 4;
	static const unsigned long RX_BUFF_SZ32 = 5;
	spi->RBCTR |= RX_D_PTR | (RX_LIM << 8) ;
	spi->RBCTR |= RX_BUFF_SZ32 << 24 | 0x10000000;	

////
/////////


	spi->CCR &= ~0xf;   //==	XMC_UART_CH_Start(XMC_UART1_CH1);	
	spi->CCR |= SPI_MODE;

//XMC_USIC_CH_SetInputSource(XMC_UART1_CH0, XMC_USIC_CH_INPUT_DX0, 2U);
	spi->DX0CR &= ~7;//  (uint32_t)((channel->DXCR[input] & (uint32_t)(~USIC_CH_DXCR_DSEL_Msk)) |
 // spi->DX0CR |= 0;//MISO = DX0_A                     
  spi->DX0CR |= 1 << 6;   //-MISO = DX0_A  synkhro 
	spi->DX0CR |= 1 << 4;   //-MISO = DX0_A  synkhro 
	unsigned short tmp;
	for(int i = 0; i < 20; ++i)
		tmp = spi->OUTR;
	isUsicIni[num] = true;
}

void ReleaseChan(unsigned char usicN, unsigned char chan)
{
	unsigned char num = usicN * USICS_NUM + chan;
	isUsicIni[num] = false;
}

void UARTini(unsigned char usicN, unsigned char chan, int rate, DX0_DSEL dx0N)
{	
	unsigned char num = usicN * USICS_NUM + chan;
	if (isUsicIni[num] != false)
		return;
	USIC_CH_TypeDef* uart = usics[num];
	Reset(uart, usicN);
	SetBaudRate(uart, rate);	
 /////
  uart->PCR_ASCMode = ((1 - 1) << 1) |// stopBits
                      (((0x10 >> 1) + 1) << 8) |   1 |  0x10000 | 0x20000;
	
	uart->SCTR = ((8 - 1) << 24) | //word lenght
								(8 - 1) << 16  |// fr.Lenght
                SHIFT_CLK_ACTIV_LEVEL | PASSIV_DATA_HIGH;
	
	uart->TCSR = (1 << 10) | 0x100;
	uart->PSCR = 0xFFFFFFFF;
	uart->CCR = NO_PARITY;//_c_onfig.parity_mode;
	
//	XMC_USIC_CH_TXFIFO_Configure(XMC_UART1_CH1, 16U, XMC_USIC_CH_FIFO_SIZE_32WORDS, 1U);
//uart->TBCTR &=  0x7000000;  //(uint32_t)//~USIC_CH_TBCTR_SIZE_Msk;// Disable FIFO 
//LOF = 0, A standard transmit buffer event occurs when the filling level equals the limit value and gets
// lower due to transmission of a data word
// STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
//  from equal to below the limit, not the fact being below
	uart->TBCTR &= ~( 0x3F |  0x3F00   | 0x7000000);//~(0x7003F3F)
  uart->TBCTR |=    16   | (1 << 8)  | 5 << 24 ;	// dataPtr == 16, limit==1, fifoSz == 32byte,	

//Disable FIFO	
//	uart->RBCTR &= ~0X7000000; //USIC_CH_RBCTR_SIZE_Msk;
//			 LOF = 1, A standard receive buffer event occurs when the filling level equals the limit value and gets bigger
//			 due to the reception of a new data word
//				
//	uart->RBCTR &= 	~(0x3f00 |			//~(USIC_CH_RBCTR_LIMIT_Msk |
//										0x3f	 |      //USIC_CH_RBCTR_DPTR_Msk |
//										0x10000000);				//USIC_CH_RBCTR_LOF_Msk) ;
	uart->RBCTR = 0;									
	static const unsigned long RX_LIM = 31;
	static const unsigned long RX_D_PTR = 0;
//	static const unsigned long RX_BUFF_SZ16 = 4;
	static const unsigned long RX_BUFF_SZ32 = 5;
	uart->RBCTR |= RX_D_PTR | (RX_LIM << 8) ;
										
	uart->RBCTR |= RX_BUFF_SZ32 << 24 | 0x10000000;
	
//											 ((limit << USIC_CH_RBCTR_LIMIT_Pos) |
//											 (data_pointer << USIC_CH_RBCTR_DPTR_Pos) |
//											 ((uint32_t)size << USIC_CH_RBCTR_SIZE_Pos) |
//											 (uint32_t)USIC_CH_RBCTR_LOF_Msk));

	
//			void XMC_USIC_CH_RXFIFO_Configure(XMC_USIC_CH_t *const channel,
//																			const uint32_t data_pointer,
//																			const XMC_USIC_CH_FIFO_SIZE_t size,
//																			const uint32_t limit)
//		//XMC_USIC_CH_RXFIFO_Configure(XMC_UART1_CH0,  0U, XMC_USIC_CH_FIFO_SIZE_16WORDS,  15U)																		

	
	uart->CCR &= ~0xf;   //==	XMC_UART_CH_Start(XMC_UART1_CH1);	
	uart->CCR |= UART_MODE;

//XMC_USIC_CH_SetInputSource(XMC_UART1_CH0, XMC_USIC_CH_INPUT_DX0, 2U);
	uart->DX0CR &= ~7;//  (uint32_t)((channel->DXCR[input] & (uint32_t)(~USIC_CH_DXCR_DSEL_Msk)) |
  uart->DX0CR |= dx0N;//
	unsigned short tmp;
	for(int i = 0; i < 20; ++i)
		tmp = uart->OUTR;
	isUsicIni[num] = true;
}

static inline bool IsRxFIFOEmpty(USIC_CH_TypeDef* usic)
{
	return ( (usic->RBUFSR & RX_DATA_READY) == 0)? true : false; 
}


unsigned  int GetRxBuffIn(USIC_CH_TypeDef* usic)
{
	static const unsigned long MASK =  0x0000003F;
	unsigned long ptrs =  usic->TRBPTR;
	unsigned long top = (ptrs >> 16) &  MASK;
	return top; 
}
	


unsigned  int GetRxBuffOut(USIC_CH_TypeDef* usic)
{
	static const unsigned long MASK =  0x0000003F;
	unsigned long ptrs =  usic->TRBPTR;
	unsigned long bot = (ptrs >> 24) & MASK;
	return bot; 
}


unsigned  int GetRxBuffLenght(USIC_CH_TypeDef* usic)
{
	static const unsigned long MASK =  0x0000003F;
	unsigned long ptrs =  usic->TRBPTR;
	unsigned long top = (ptrs >> 16) &  MASK;
	unsigned long bot = (ptrs >> 24) & MASK;
	unsigned int ret = (top >= bot)? (top - bot) : (GetRxBuffSz(usic) - bot + top);
	return ret; 
}

unsigned int GetTxBuffLenght(USIC_CH_TypeDef* usic)
{
	static const unsigned long MASK =  0x0000003F;
	unsigned long ptrs =  usic->TRBPTR;
	unsigned long top = ptrs &  MASK;
	unsigned long bot = (ptrs >> 5) & MASK;
	unsigned int delta = (top > bot)? (top - bot) : (bot - top);
	return delta; 
}

static inline bool IsTxFIFOFilled(USIC_CH_TypeDef* usic)
{
	return ( GetTxBuffLenght (usic) < (TX_FIFO_SZ - 1) )? false : true; 
}	

bool IsTxBuffEmpty(USIC_CH_TypeDef* usic)
{
	return ( (usic->TCSR & B7) == 0 )? true : false; 
}	


static void DoNoth()//USIC_CH_TypeDef* usic, unsigned short data)
{
}

void Tx(USIC_CH_TypeDef* usic, unsigned short data)
{
	usic->IN[0] = data;
}
void Tx1(unsigned char usicN, unsigned char chan, unsigned short data)
{
	Tx(usics[usicN * USICS_NUM + chan], data);
	//USIC_CH_TypeDef* uart = usics[usicN * USICS_NUM + chan];
}

static const unsigned int LIMIT = 10000000;
//Works until it transmit or puts all the data in the FIFO or completes on a timeout
//Returns the number of words actually transferred and puts to the FIFO
unsigned int SyncUSICTx(USIC_CH_TypeDef* usic, const unsigned short* data, unsigned int len)
{
	signed int timeout = LIMIT;
	int nTx;
	for (nTx = 0; (nTx < len) && (timeout > 0) ; )
	{
		timeout = LIMIT;
		while ( IsTxFIFOFilled(usic) && (--timeout > 0) )
		{
		}
			//timeIsUp = (--timeout > 0)? false : true; 
		(timeout == 0)? DoNoth() : Tx(usic, data[nTx++]); 
	}
	return nTx;
}

//Works until it fills the FIFO. 
//After the buffer is filled, returns the number of words that could be transferred and placed in the FIFO.
unsigned int ASyncUSICTxw(USIC_CH_TypeDef* usic, const unsigned short* data, unsigned int len)
{
	int nTx;
	for (nTx = 0; (nTx < len) && !IsTxFIFOFilled(usic) ; )
		Tx(usic, data[nTx++]); 
	return nTx;
}


unsigned int ASyncUSICTxb(USIC_CH_TypeDef* usic, const char* data, unsigned int len)
{
	int nTx;
	for (nTx = 0; (nTx < len) && !IsTxFIFOFilled(usic) ; )
		Tx(usic, data[nTx++]); 
	return nTx;
}

unsigned int USICRxw(USIC_CH_TypeDef* usic, unsigned short* data)
{	
//	unsigned int num = 0;
//	while (GetRxBuffLenght(usic) )/////!IsRxFIFOEmpty(usic))
//	{
//		//data[num++] = usic->RBUF;	
//	//	for (int i = 0; i < 6; ++i)
//		//{
//			data[num++] = usic->OUTR;	
////			data[num++] = usic->OUTR;
//		//}
//	}
//	return num;
	
	unsigned int num = 0, ret = GetRxBuffLenght(usic);
//	while (num <= ret )/////!IsRxFIFOEmpty(usic)
	for (; num < 64; ++num)
		data[num] = (unsigned short) (usic->OUTR);
	data[47] = 0xabcd;
	//data[num++] = (unsigned short) (usic->OUTR);
	//data[num++] = (char) (usic->RBUF & 0xff);	
	return ret;
	
}



void FastUSICTxw(USIC_CH_TypeDef* usic, const unsigned short* data, unsigned int len)
{
	int nTx;
	for (nTx = 0; len > 0 ; --len )
		Tx(usic, data[nTx++]); 
} 


bool SPIdeviceConf(USIC_CH_TypeDef* usic, unsigned long frLen, unsigned long wLen, unsigned char csNum, const CSpol csPolar, const BitOrder btOrder)
{
//TX Buffer ready check
	bool ret;
	if(IsTxBuffEmpty(usic))
	{
		ret = true;
		usic->PCR = 3 |  csPolar | CS_CODE[csNum];
		usic->SCTR = btOrder | PASSIV_DATA_HIGH | SHIFT_CLK_ACTIV_LEVEL | (frLen - 1) << 16 |  (wLen - 1) << 24;
	}
	else
		ret = false;
	return ret;
}





unsigned short FIFORead(USIC_CH_TypeDef* usic)
{
		return usic->OUTR;
}

void USICRxFIFOClean(USIC_CH_TypeDef* usic)
{
	unsigned short tmp;
	while (!IsRxFIFOEmpty(usic))
		tmp = usic->OUTR;	
	tmp = usic->OUTR;	
	tmp = usic->OUTR;	
}

unsigned int USICRxb(USIC_CH_TypeDef* usic, char* data)
{
	unsigned int num = 0; //ret = GetRxBuffLenght(usic);

	while (!IsRxFIFOEmpty(usic) )/////!IsRxFIFOEmpty(usic)
	{
			data[num++] = (char) (usic->OUTR & 0xff);
		data[num] = 'x';
	}
	
	//data[num++] = (char) (usic->RBUF & 0xff);	
	return num;
}






unsigned char GetRxBuffSz(USIC_CH_TypeDef* usic)
{
	unsigned long ret =  (usic->RBCTR & USIC_CH_RBCTR_SIZE_Msk) >> 24;
	return (ret == 0)? 0 : (unsigned char) (1 << ret);
}	

//	spi->KSCFG = 3; // Module enable & bit protection
////==Clock==
//	spi->CCR &= 0xFFFFFFFF00;//usic disable
//	spi->FDR = 1 << 14 | 0x3FE; //0x3fe == 115200
//	spi->BRG &= 0xFFFC0083EF;
//	spi->BRG = PASSIV_CLK_HIGH | 0xf << 10;// | 0 << 16; //Time Quanta = 15, divider = 1;
//	
////==Shift control	
//	spi->SCTR = (wLen  - 1) << 24 | (frLen - 1) << 16 | SHIFT_CLK_ACTIV_LEVEL  | PASSIV_DATA_HIGH | MS_BIT_FIRST;
////Tx control
//	spi->TCSR = 1 << 12 | TX_START | SINGLE_SHOT_MODE;	

//	/////	
//	
////SPI protocol
//	spi->PCR = ACTIV_LOW_SLAVE_SEL | DIRECT_SLAVE_SEL | GENERATE_SLAVE_SEL;		
////Clear Flags
//	spi->PSCR = 0xFFFFFFFF;
//	spi->CCR = NO_PARITY;
//	spi->DX0CR = 0x40;// MISO = P22 = DX0A, rising edge is active	
//	spi->TBCTR &= 0xF8FFC0C0;
//	spi->TBCTR = TX_FIFO_LIMIT | FIFO_TRIG_MODE | FIFO_TRIG_EN ;	
//	spi->RBCTR = 32 |  RX_FIFO_LIMIT | FIFO_TRIG_MODE | FIFO_TRIG_EN;
//	spi->TBCTR |= FIFO_SZ_32;
//	spi->RBCTR |= FIFO_SZ_32;
//	spi->CCR |= SPI_MODE;//usic start in SPI mode
//	
//	//////
//	
//	
//	
//	
//	
//	USIC_CH_TypeDef* uart = usics[usicN * USICS_NUM + chan];
//	Reset(uart, usicN);
//	SetBaudRate(uart, rate);	

//  uart->PCR_ASCMode = ((1 - 1) << 1) |// stopBits
//                      (((0x10 >> 1) + 1) << 8) |   1 |  0x10000 | 0x20000;
// 
//	uart->SCTR = ((8 - 1) << 24) | //word lenght
//								(8 - 1) << 16  |// fr.Lenght
//                SHIFT_CLK_ACTIV_LEVEL     | PASSIV_DATA_HIGH;
//	
//	uart->TCSR = (1 << 10) | 0x100;
//	uart->PSCR = 0xFFFFFFFF;
//	uart->CCR = NO_PARITY;//_c_onfig.parity_mode;
//	/////	
////	XMC_USIC_CH_TXFIFO_Configure(XMC_UART1_CH1, 16U, XMC_USIC_CH_FIFO_SIZE_32WORDS, 1U);
////uart->TBCTR &=  0x7000000;  //(uint32_t)//~USIC_CH_TBCTR_SIZE_Msk;// Disable FIFO 
////LOF = 0, A standard transmit buffer event occurs when the filling level equals the limit value and gets
//// lower due to transmission of a data word
//// STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
////  from equal to below the limit, not the fact being below
//	uart->RBCTR = 0;									
//	static const unsigned long RX_LIM = 31;
//	static const unsigned long RX_D_PTR = 0;
////	static const unsigned long RX_BUFF_SZ16 = 4;
//	static const unsigned long RX_BUFF_SZ32 = 5;
//	uart->RBCTR |= RX_D_PTR | (RX_LIM << 8) ;
//										
//	uart->RBCTR |= RX_BUFF_SZ32 << 24 | 0x10000000;
//	
////											 ((limit << USIC_CH_RBCTR_LIMIT_Pos) |
////											 (data_pointer << USIC_CH_RBCTR_DPTR_Pos) |
////											 ((uint32_t)size << USIC_CH_RBCTR_SIZE_Pos) |
////											 (uint32_t)USIC_CH_RBCTR_LOF_Msk));

//	
////			void XMC_USIC_CH_RXFIFO_Configure(XMC_USIC_CH_t *const channel,
////																			const uint32_t data_pointer,
////																			const XMC_USIC_CH_FIFO_SIZE_t size,
////																			const uint32_t limit)
////		//XMC_USIC_CH_RXFIFO_Configure(XMC_UART1_CH0,  0U, XMC_USIC_CH_FIFO_SIZE_16WORDS,  15U)																		

























