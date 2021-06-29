#ifndef STR_H
#define STR_H

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
