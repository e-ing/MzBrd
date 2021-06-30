/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): Blinky example for XMC4300
 *----------------------------------------------------------------------------
 * Blink LED connected with P4.0, P4.1
 * XMC4300 Relax kit
 *
 *----------------------------------------------------------------------------*/
//D:\DC\!W\Prj\SW\XMC4300\XMC4300 Relax Kit\Blinky\main3.cpp
#include  <stdio.h>
#include  <XMC4300.h>
#include  <TheTime.h>
#include  <USIC.h>
#include <GPIOs.h>
#include <AD5761.h>
#include <ADS8694.h>
#include <BoardDev.h>
#include <Str.h>


int main (void) 
{
  SystemCoreClockUpdate();                     // /* update SystemCoreClock     */
  if (SysTick_Config(SystemCoreClock / 1000)) //* SysTick 1 msec interrupts  */
    while (1) ; 	///* Capture error              */
	PoutPort*  hbPin =  GetYellowLED();// =  {GPs[GPP2], 0, 0, 0, 0};
	
	PoutPort*  hbBlue =  GetBlueLED();
	
	for(int i = 0; i < 6; ++i)
	{
	//	Tx1(1, 1, '.');
		GPToggle( hbPin );
		Delay(250 - i * i * i);
	}
	
	for(int i = 0; i < 6; ++i)
	{
	//	Tx1(1, 1, '.');
		GPToggle( hbBlue );
		Delay(250 - i * i * i);
	}
	PinPort rx1 = {GPs[GPP0], 0, 0};
	GPinPushPull(&rx1);
	UARTini(1, 1, 115200, DXD);
	PoutPort  tx1 =  {GPs[GPP0], 1, 0, 0, 0};
	IniGPO( &tx1);
	SetAltFn(&tx1, 2);
	SendStr("UART1 on \n");
	USIC_CH_TypeDef* uart1 = usics[3];
	USIC_CH_TypeDef* uart2 = usics[2];
	
	PinPort rx2 = {GPs[GPP0], 4, 0};
	GPinPushPull(&rx2);
	UARTini(1, 0, 115200, DXA);
	PoutPort  tx2 =  {GPs[GPP0], 5, 0, 0, 0};
	IniGPO( &tx2);
	SetAltFn(&tx2, 2);
	SendStr("UART2 on \n");
	SendStr2("==UART2 on \n");

		SendStr("UART2 on \n");
		SendStr2("UART2==n \n");
		Delay(200);
		SendStr2("...");
		Delay(200);

		GPToggle(hbBlue);

	
	
	PinPort miso = {GPs[GPP2], 2, 0};
	GPinPushPull(&miso);
//	SPIini(0, 1, 16, 32, 1000000);
	SPIDev dac = {0, 1, 3, 0};
	SPIDev adc = {0, 1, 0, 0};
	ADS8694 ads = { &adc, {0}, ADS_IDDLE };
	
	PoutPort mosi = {GPs[GPP2], 5, 0, 0, 0};
	IniGPO(&mosi);
	SetAltFn(&mosi, 2);
	
	PoutPort sclk = {GPs[GPP2], 4, 0, 0, 0};
	IniGPO(&sclk);
	SetAltFn(&sclk, 2);
	
	PoutPort csDac = {GPs[GPP1], 13, 0, 0, 0};
	IniGPO(&csDac);
	SetAltFn(&csDac, 2);//selo3
	
	PoutPort lDac = {GPs[GPP1], 14, 0, 0, 0};
	IniGPO(&lDac);//csADC
	GPtoL(&lDac);
	
	PoutPort csAdc = {GPs[GPP2], 3, 0, 0, 0};
	IniGPO(&csAdc);//csADC
	SetAltFn(&csAdc, 2);//selo0
	
	AD5761ini(&dac);
	SendStr("DAC AD5761 on \n");
	SendStr2("==DAC AD5761 on \n");
	ADS8694Ini(&ads);
	SendStr("ADC ADS8494 on \n");
	SendStr2("ADC ADS8494 on \n");

	

	
//	USIC_CH_TypeDef* spi1 = usics[1];
	
	int cnt = 0;
	for(int i = 0; i < 10; ++i)
		Tx1(1, 1, '=');
	for(int i = 0; i < 6; ++i)
	{
		Tx1(1, 1, '.');
		GPToggle( hbPin );
		Delay(250 - i * i * i);
	}
	unsigned char nByte = 0;
	char rxData[64];
	Enter();
	SendStr("Hello! Mz0.0\n");
	int daVal = 0;
	bool isRise = true;
	int delta = 1000;
	int max = 63000, min = 2000;
	unsigned short regRdD[64];
	int adRd[16];
	int rxSize = 0;
//	unsigned short xyz[] = {0x5555, 0xAAAA};
	int tmpM, tmp2, tmpL, nWrd, adD, adCh, adAny, orjL,  orjMost;
	ADS8694ReadStart(&ads, adRd);
	while(1)
	{
		//Read UART
		if((++cnt % 25) == 0)
		{
			GPToggle(hbPin);
		}
		if((cnt % 147) == 0)
		{
			GPToggle(hbBlue);
			if (ADS8694Ch0ReadStart(&ads, adRd) != false)
			{ 
				SendLn("ok #", cnt / 147, DEC);
				Send("", adRd[0], HEX);				
				adD = ((adRd[2] >> 2) & 0x3f) | ((adRd[1] & 0xff) << 10);
//				Send("-", adRd[1], HEX);
//				Send("-", adRd[2], HEX);
				SendLn("-", adD, HEX);
				Sleep(2000);
			}
		}
	}	
	Delay(5);
	//	SendLn("------------------", ++cnt, DEC);

//		SPIdeviceConf ( spi1, 32, 16, 3, (const  CSpol) 4, (const BitOrder) 1);                                                                                                                                                                                         , CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
		//SPIdeviceConf(spi, FRAME_L, WORD_L, slN, CSPOL_LOW, MSB_FIRST))//TX Buffer ready check
		//unsigned short data[2] = {(unsigned short)(0x >> 12) | rgAdr << 4,
		//													(unsigned short)(val & 0xFFF)};		
//		USICRxFIFOClean(spi1);
	
	//	else if((++cnt % 5) == 0)
					
		//FastUSICTxw(spi1, xyz, 2);	
//		AD5761SetVal(&dac, daVal);
		daVal += isRise? delta : -delta;
		isRise = (daVal > max)? false : (daVal < min)? true : isRise;
}

//ReadDAC
//						{
//							GPToggle(hbBlue);
//							SendLn ("-------------------",  cnt, DEC);
//							Send("t", GetRxBuffIn(dac.spi), DEC);
//							Send(" b", GetRxBuffOut(dac.spi), DEC);
//							SendLn(" #", GetRxBuffLenght(dac.spi), DEC);
//							SendLn ("-------------------",  cnt, DEC);
//							for (int i = 0; i < 128; ++i)
//							{
//								AD5761ReadReg(&dac, RD_CTL_RG, regRdD);
//								Send("t", GetRxBuffIn(dac.spi), DEC);
//								Send(" b", GetRxBuffOut(dac.spi), DEC);
//								SendLn(" #", GetRxBuffLenght(dac.spi), DEC);
//				//						for (int i = 0; i < 4; ++i)
//				//						{
//				//							Send(" d[", i, DEC);
//				//							SendLn("]", FIFORead(dac.spi), HEX);
//				//						}
//				//						SendLn("+++++++++++++++++++", cnt, DEC);
//								Send("m", regRdD[0], HEX);
//								Send(".", regRdD[1], HEX);
//				//				Send(".", regRdD[2], HEX);
//				//				SendLn("-", regRdD[3], HEX);	
//								Sleep(500);
//							}
//						}


//Read UART
//					nByte = USICRxb(uart1, rxData);
//					if(nByte > 0)
//					{
//						//GPToggle(GPs[GPP2], 0);
//						Enter();
//			//			SendLn("TRBPTR", uart1->TRBPTR , HEX);
//			//			SendLn("TRBSR= ", uart1->TRBSR , HEX);
//						SendLn("RxFIFO lenght is", nByte, DEC);
//						for(int i = 0; i <= nByte; ++i)
//						{
//							Send("str[", i, DEC);
//							SendLn("]=", rxData[i], HEX);
//						}
//						for(int i = 0; i <= nByte; ++i)
//						{
//							Tx1(1, 1, rxData[i]);
//						}
//						SendLn("------------------", ++cnt, DEC);
//					}




//					nByte = USICRxb(uart2, rxData);
//					if(nByte > 0)
//					{
//						//GPToggle(GPs[GPP2], 0);
//						Enter();
//			//			SendLn("TRBPTR", uart1->TRBPTR , HEX);
//			//			SendLn("TRBSR= ", uart1->TRBSR , HEX);
//						SendLn2("RxLn ", nByte, DEC);
//						Delay(100);
//						rxData[nByte] = 0;
//						SendStr2(rxData);
////						for(int i = 0; i <= nByte; ++i)
////						{
////						//	Send2("str[", i, DEC);
////							SendLn2("-", rxData[i], HEX);
////						}
////						for(int i = 0; i <= nByte; ++i)
////						{
////							Tx1(1, 0, rxData[i]);
////						}
////						SendLn2("---", ++cnt, DEC);
//					}


/////////////////////////====================//////////////////

///*----------------------------------------------------------------------------
//  initialize LED Pins
// *----------------------------------------------------------------------------*/
//void LED_Init (void) {

//  /* configure PORT P4 pin as output */
//  PORT2->IOCR0  &=  ~(0xFFUL << 0);          /* clear P4.0 settings           */
//  PORT2->IOCR0  |=   (0x80UL << 0);          /* P4.0 output push pull         */
//  PORT2->PDR0   &=  ~(0x07UL << 0);          /* clear P4.0 output settings    */
//  PORT2->PDR0   |=   (0x02UL << 0);          /* P4.0 output strong            */

////  PORT2->IOCR0  &=  ~(0xFFUL << 8);          /* clear P4.1 settings           */
////  PORT2->IOCR0  |=   (0x80UL << 8);          /* P4.1 output push pull         */
////  PORT2->PDR0   &=  ~(0x07UL << 4);          /* clear P4.1 output settings    */
////  PORT2->PDR0   |=   (0x02UL << 4);          /* P4.1 output strong            */

//  LED_Off ();                               /* switch LEDs off               */

//}


///*----------------------------------------------------------------------------
//  Function that turns on requested LED
// *----------------------------------------------------------------------------*/
//void LED_On () {

//    PORT2->OMR = led_mask[0] ;  //P4.0
////   PORT4->OMR = led_mask[1] ;  //P4.1

//}

///*----------------------------------------------------------------------------
//  Function that turns off requested LED
// *----------------------------------------------------------------------------*/
//void LED_Off () {

//    PORT2->OMR = (led_mask[0] << 16);  //P4.0
////    PORT4->OMR = (led_mask[1] << 16);  //P4.1
//}







//#include  <math.h>

//const unsigned long led_mask[2] = {1UL << 0, 1UL << 1};      /*  GPIO P4.0, GPIO P4.1       */

//void LED_Init(void);
//void LED_On  (void);
//void LED_Off (void);


//volatile uint32_t msTicks;  

//void SysTick_Handler(void) {
//  msTicks++;
//}


//void Delay (uint32_t dlyTicks) {
//  uint32_t curTicks;

//  curTicks = msTicks;
//  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
//}




//////////////////===================///////////////////////




//int main (void)
//{
////	void (*Run)(void);

////	Run = JumpTo;//  0x0C000000;
////	Run =  (void(*)(void)) 0x0C000000;
////  SystemCoreClockUpdate();                      /* update SystemCoreClock     */
////  if (SysTick_Config(SystemCoreClock / 1000)) 
////		
////	{ 																						/* SysTick 1 msec interrupts  */
////    while (true)                          /* Capture error              */
////		{
////		}
////  }
//	
//	  SystemCoreClockUpdate();                      /* update SystemCoreClock     */
//  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
//    while (1) __NOP();                          /* Capture error              */
//  }

//	
//	IniGPO(GPs[GPP2], 0);
//	while(true)
//	{
//		Delay(1000);
//		GPToggle(GPs[GPP2], 0);		
//	}
//	
	

 // LED_Init();
//	GPout hbBit(GPP5, 3);
//	AbstrBitOut* hbBit = new GPout (GPP2, 7);
//	AbstrBitOut* indBit = new GPout (GPP2, 8);
//	AbstrBitIn* tst = new GPin (GPP2, 9);
//	bool x = true;
////SPI
//	AbstrBitOut* adCS = new GPout(GPP2, 3);
//	*adCS = 1;
//	AbstrBitOut* daCS = new GPout(GPP1, 13);
//	*daCS = 1;
//	AbstrBitOut* ldac = new GPout(GPP1, 14);
//	*ldac = 1;
//	AbstrBitOut* mosi = new GPout(GPP2, 5);
//	AbstrBitOut* sclk = new GPout(GPP2, 4);
//	AbstrBitIn* miso = new GPin(GPP2, 2);
//	*sclk = *mosi = false;
//	*daCS = *miso;
//	AbstrBitOut& any = *daCS;
//	any = 0;
//	
//	
//	adCS->SetAltFn(2);
//	daCS->SetAltFn(2);
//	mosi->SetAltFn(2);
//	sclk->SetAltFn(2);
//	

	
//	
//  while(true)
//	{                               
// //   LED_On ();
//		*hbBit = 1;
//    Delay(500);                              
//		!*hbBit;
//    //LED_Off();                               
//    Delay(500);                              
//		*hbBit = x;
//		Delay(100);
//		*hbBit = false;
//		Delay(200);
//		*indBit = !tst;	
//  }

//}


///////////////////////////==========================//////////////////

//#include <xmc_uart.h>
//const XMC_UART_CH_CONFIG_t _c_onfig =
//{
//  .baudrate      = 115200,//19200U,
//  .data_bits     = 8,
//  .frame_length  = 8,
//  .stop_bits     = 1,
//  .oversampling  = 16,
//  .parity_mode   = XMC_USIC_CH_PARITY_MODE_NONE
//};


////void XMC_USIC_CH_TXFIFO_Configure(XMC_USIC_CH_t *const channel,
////                                  const uint32_t data_pointer,
////                                  const XMC_USIC_CH_FIFO_SIZE_t size,
////                                  const uint32_t limit)
////{
////  /* Disable FIFO */
////  channel->TBCTR &= (uint32_t)~USIC_CH_TBCTR_SIZE_Msk;

////  /* LOF = 0, A standard transmit buffer event occurs when the filling level equals the limit value and gets
////   * lower due to transmission of a data word
////   * STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
////   *  from equal to below the limit, not the fact being below
////   */
////  channel->TBCTR = (uint32_t)(channel->TBCTR & (uint32_t)~(USIC_CH_TBCTR_LIMIT_Msk |
////                                                           USIC_CH_TBCTR_DPTR_Msk |
////                                                           USIC_CH_TBCTR_SIZE_Msk)) |
////                   (uint32_t)((limit << USIC_CH_TBCTR_LIMIT_Pos) |
////                   (data_pointer << USIC_CH_TBCTR_DPTR_Pos) |
////                   ((uint32_t)size << USIC_CH_TBCTR_SIZE_Pos));
////}



////				void XMC_UART_CH_Init(XMC_USIC_CH_t *channel, const XMC_UART_CH_CONFIG_t *const config)
////				{
////					uint32_t oversampling = XMC_UART_CH_OVERSAMPLING;

////					/* USIC channel switched on*/
////					XMC_USIC_CH_Enable(channel);
////					
////					if(config->oversampling != 0U)
////					{
////						oversampling = (uint32_t)config->oversampling;
////					}
////					
////					/* Configure baud rate */
////					if (config->normal_divider_mode)
////					{
////						/* Normal divider mode */
////						(void)XMC_USIC_CH_SetBaudrateEx(channel, config->baudrate, oversampling);
////					}
////					else
////					{
////						/* Fractional divider mode */
////						(void)XMC_USIC_CH_SetBaudrate(channel, config->baudrate, oversampling);
////					}
////						
////					/* Configure frame format
////					 * Configure the number of stop bits
////					 * Pulse length is set to 0 to have standard UART signaling, 
////					 * i.e. the 0 level is signaled during the complete bit time
////					 * Sampling point set equal to the half of the oversampling period
////					 * Enable Sample Majority Decision 
////					 * Enable Transfer Status BUSY
////					 */
////					channel->PCR_ASCMode = (uint32_t)(((config->stop_bits - 1UL) << USIC_CH_PCR_ASCMode_STPB_Pos) |
////																						(((oversampling >> 1UL) + 1UL) << USIC_CH_PCR_ASCMode_SP_Pos) |
////																						USIC_CH_PCR_ASCMode_SMD_Msk |
////																						USIC_CH_PCR_ASCMode_RSTEN_Msk | USIC_CH_PCR_ASCMode_TSTEN_Msk);

////					/* Set passive data level, high
////						 Set word length. Data bits - 1
////						 If frame length is > 0, frame_lemgth-1; else, FLE = WLE (Data bits - 1)
////						 Transmission Mode: The shift control signal is considered active if it
////						 is at 1-level. This is the setting to be programmed to allow data transfers */
////					channel->SCTR = (uint32_t)((((uint32_t)config->data_bits - 1UL) << USIC_CH_SCTR_WLE_Pos) |
////																		 ((0x1UL << USIC_CH_SCTR_TRM_Pos) | USIC_CH_SCTR_PDL_Msk));

////					if (config->frame_length != 0U)
////					{
////						channel->SCTR |= (uint32_t)(((uint32_t)config->frame_length - 1UL) << USIC_CH_SCTR_FLE_Pos);
////					}
////					else
////					{
////						channel->SCTR |= (uint32_t)(((uint32_t)config->data_bits - 1UL) << USIC_CH_SCTR_FLE_Pos);
////					}

////					/* Enable transfer buffer */
////					channel->TCSR = (0x1UL << USIC_CH_TCSR_TDEN_Pos) |
////													USIC_CH_TCSR_TDSSM_Msk;

////					/* Clear protocol status */
////					channel->PSCR = 0xFFFFFFFFUL;

////					/* Set parity settings */
////					channel->CCR = (uint32_t)config->parity_mode;
////				}



//#include <stdlib.h>

//extern unsigned long oVeRSampling;
//extern unsigned long __s_1, __msk_1, __msk_2, __msk_3;
//void MyUART_ini(unsigned char usicN, unsigned char chan)
//{
//	USIC_CH_TypeDef* uart = usics[usicN * USICS_NUM + chan];
//	if(usicN == 0)
//	{
//		*(unsigned long *)&(SCU_RESET->PRCLR0) = 0x800;
//    while (  *(unsigned long *)&(SCU_RESET->PRSTAT0) & 0x800    )///XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_USIC1));//XMC_SCU_PERIPHERAL_RESET_USIC1
//		{
//		}	
//	}
//	else if(usicN == 1)
//	{
//  *(unsigned long *)(&(SCU_RESET->PRCLR0) + 3 ) = 0x80;
//	

//    while (  *(unsigned long *)(&(SCU_RESET->PRSTAT0) + 3) & 0x80    )///XMC_SCU_RESET_IsPeripheralResetAsserted(XMC_SCU_PERIPHERAL_RESET_USIC1));//XMC_SCU_PERIPHERAL_RESET_USIC1
//		{
//		}
//	}

//	uart->KSCFG = 3; // Module enable & bit protection
//	while ((uart->KSCFG & 1) == 0)//USIC_CH_KSCFG_MODEN_Msk) == 0U)
//	{
//	}
//	//==Clock==
//	uart->CCR &= 0xFFFFFFFF00;// Set USIC channel in IDLE mode
//	
//	unsigned int overS = 16;
//	unsigned int  prphClk = (SystemCoreClock >>
//									 ((SCU_CLK->PBCLKCR & SCU_CLK_PBCLKCR_PBDIV_Msk) >> SCU_CLK_PBCLKCR_PBDIV_Pos)); 
//	//	unsigned int rate = _c_onfig.baudrate;
//		signed int rateUpp, rateLo, rate = _c_onfig.baudrate;
//		unsigned int brgClk = rate * overS, pdiv = 1,  divStp;
//		
//		if (prphClk > brgClk)
//		{
//			divStp = prphClk / brgClk; // integer division gets truncated
//			while (divStp >= 1023)
//			{
//				brgClk = rate * overS * ++pdiv;
//				divStp = prphClk / brgClk; // integer division gets truncated
//			}
//								rateUpp = prphClk / (divStp * overS * pdiv);
//								rateLo = prphClk / ((divStp + 1) * overS * pdiv);
//					//    // choose better approximation if the peripheral frequency is not a multiple of the baudrate
//							if (abs (rate - rateLo) < abs(rate - rateUpp))
//								 ++divStp;
//							divStp = 1024 - divStp;


//							uart->FDR = (1 << 14) | divStp;															//XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_NORMAL |
//																																			//(divStp //<< USIC_CH_FDR_STEP_Pos == 0);
//							uart->BRG &= ~( 0x7C00						 |//~USIC_CH_BRG_DCTQ_Msk
//															0x3ff0000          | // USIC_CH_BRG_PDIV_Msk |
//													    0x300	             | // USIC_CH_BRG_PCTQ_Msk |
//													    0x10 );               //USIC_CH_BRG_PPPEN_Msk
//							uart->BRG |=  ((overS - 1) << 10) |	// USIC_CH_BRG_DCTQ_Pos)
//														((pdiv -1) <<  16);    //USIC_CH_BRG_PDIV_Pos)
//							

////					//    uart->BRG = (channel->BRG & ~(USIC_CH_BRG_DCTQ_Msk |
////					//                                     USIC_CH_BRG_PDIV_Msk |
////					//                                     USIC_CH_BRG_PCTQ_Msk |
////					//                                     USIC_CH_BRG_PPPEN_Msk)) |
////					//                   ((oversampling - 1U) << USIC_CH_BRG_DCTQ_Pos) |
////					//				   ((pdiv -1) << USIC_CH_BRG_PDIV_Pos);

////					//    status = XMC_USIC_CH_STATUS_OK;
//		}
//	
//	
////	(void) XMC_USIC_CH_SetBaudrateEx(uart, _c_onfig.baudrate, 16);
//	
////	XMC_USIC_CH_STATUS_t XMC_USIC_CH_SetBaudrateEx(XMC_USIC_CH_t *const channel, int32_t rate, int32_t oversampling)
////{
////  int32_t peripheral_clock = XMC_SCU_CLOCK_GetPeripheralClockFrequency();
////  int32_t brg_clock = rate * oversampling;
////  int32_t actual_rate_upper;
////  int32_t actual_rate_lower;
////  uint32_t pdiv = 1;
////  uint32_t divider_step;
////  XMC_USIC_CH_STATUS_t status;

////  if (peripheral_clock > brg_clock)
////  {
////    divider_step = peripheral_clock / brg_clock; // integer division gets truncated
////    while (divider_step >= 1023)
////    {
////      pdiv++;
////      brg_clock = rate * oversampling * pdiv;
////      divider_step = peripheral_clock / brg_clock; // integer division gets truncated
////    }
////    actual_rate_upper = peripheral_clock / (divider_step * oversampling * pdiv);
////    actual_rate_lower = peripheral_clock / ((divider_step + 1) * oversampling * pdiv);

////    // choose better approximation if the peripheral frequency is not a multiple of the baudrate
////    if (abs(rate - actual_rate_lower) < abs(rate - actual_rate_upper))
////    {
////      divider_step += 1;
////    }

////    divider_step = 1024 - divider_step;


////    channel->FDR = XMC_USIC_CH_BRG_CLOCK_DIVIDER_MODE_NORMAL |
////                   (divider_step << USIC_CH_FDR_STEP_Pos);

////    channel->BRG = (channel->BRG & ~(USIC_CH_BRG_DCTQ_Msk |
////                                     USIC_CH_BRG_PDIV_Msk |
////                                     USIC_CH_BRG_PCTQ_Msk |
////                                     USIC_CH_BRG_PPPEN_Msk)) |
////                   ((oversampling - 1U) << USIC_CH_BRG_DCTQ_Pos) |
////				   ((pdiv -1) << USIC_CH_BRG_PDIV_Pos);

////    status = XMC_USIC_CH_STATUS_OK;
////  }
////  else
////  {
////    status = XMC_USIC_CH_STATUS_ERROR;
////  }

////  return status;
////}

//	
//	
//	
//	
//	//XMC_UART_CH_Init(XMC_USIC1_CH1, &_c_onfig);
//	
//	
//	
//	
//	
//	
//	
// ///// Configure frame format
// //// Configure the number of stop bits
// //// Pulse length is set to 0 to have standard UART signaling, 
// //// i.e. the 0 level is signaled during the complete bit time
// //// Sampling point set equal to the half of the oversampling period
// //// Enable Sample Majority Decision 
// //// Enable Transfer Status BUSY
// /////
//  uart->PCR_ASCMode = ((1 - 1) << 1) |// stopBits
//                      (((0x10 >> 1) + 1) << 8) |   1 |  0x10000 | 0x20000;
//	
//	
//	uart->SCTR = ((8 - 1) << 24) | //word lenght
//								(8 - 1) << 16  |// fr.Lenght
//                SHIFT_CLK_ACTIV_LEVEL     | PASSIV_DATA_HIGH;
//	
//	uart->TCSR = (1 << 10) | 0x100;
//	uart->PSCR = 0xFFFFFFFF;
//	uart->CCR = NO_PARITY;//_c_onfig.parity_mode;
//	
//	//	XMC_USIC_CH_TXFIFO_Configure(XMC_UART1_CH1, 16U, XMC_USIC_CH_FIFO_SIZE_32WORDS, 1U);
//	//uart->TBCTR &=  0x7000000;  //(uint32_t)//~USIC_CH_TBCTR_SIZE_Msk;// Disable FIFO 
////LOF = 0, A standard transmit buffer event occurs when the filling level equals the limit value and gets
//// lower due to transmission of a data word
//// STBTEN = 0, the trigger of the standard transmit buffer event is based on the transition of the fill level
////  from equal to below the limit, not the fact being below
//	 uart->TBCTR &= ~( 0x3F |  0x3F00   | 0x7000000);
//	 uart->TBCTR |=    16   | (1 << 8)  | 5 << 24 ;	// dataPtr == 16, limit==1, fifoSz == 32byte,
//	
//	
//	uart->CCR &= ~0xf;   //==	XMC_UART_CH_Start(XMC_UART1_CH1);	
//	uart->CCR |= UART_MODE;
//	IniGPO(GPs[GPP0], 1);
//	SetAltFn(GPs[GPP0], 1, 2);

//}	

