/*---------------------------------------------------------------------------
;
; 	FILE NAME  	: TP2_Lib_Timers_UART.H
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configuration pour le TP BSE
;
; 	REVISION 1.0
;
;---------------------------------------------------------------------------*/

#ifndef __TP2_Lib_Timers_UART__
#define __TP2_Lib_Timers_UART__



#define SYSCLK 22118400L
#define BAUDRATE 115200L
#define Freq_timer0 (BAUDRATE*16)

#define Preload_Timer1 (SYSCLK/Freq_timer0)
#if Preload_Timer1 > 255 
#error "Valeur Preload Timer0 HORS SPECIFICATIONS"
#endif 

void Config_Timer2_TimeBase(void);
void Config_Timer4_Event_Counter(unsigned int);
void CFG_Clock_UART0(void);
void CFG_UART0(void);
char putchar(char c);
char _getkey(void);
char getkey_one_time(void);

#endif