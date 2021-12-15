/*---------------------------------------------------------------------------
;
; 	FILE NAME  	: TP2_BSE_Main.H
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configuration pour le TP BSE
;
; 	REVISION 1.0
;
;---------------------------------------------------------------------------*/

#ifndef __TP2_BSE_Main__
#define __TP2_BSE_Main__


void Config_INT7 (void);
void Config_INT6 (void);
void Config_Timer2_TimeBase(void);
void Modif_Cfg_Globale (void);
void Oscillator_Init();
void Config_Timer4_Event_Counter(void);
void CFG_Clock_UART0(void);
void CFG_UART0(void);
char getkey_one_time(void);
char Putchar(char c);
void plusUn(char *buf);
#endif