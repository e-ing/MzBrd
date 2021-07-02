#ifndef STR_H
#define STR_H

#include <USIC.h>

//Put ascii string recived from uart to str, add 0 to string end
//Return number of char recived from uart
unsigned int GetStr(USIC_CH_TypeDef* uart, char* str);


void Enter(void);

void SendStr(const char* str);

void SendStr2(const char* str);


typedef enum  
{
	 HEX = 'h',
   DEC = 'd'	
}  PrintMode ;

void Send(const char* str, unsigned long data, PrintMode mode);

void SendLn(const char* str, unsigned long data, PrintMode mode);

void Send2(const char* str, unsigned long data, PrintMode mode);

void SendLn2(const char* str, unsigned long data, PrintMode mode);

#endif
