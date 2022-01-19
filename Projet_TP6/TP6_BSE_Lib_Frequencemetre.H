/*---------------------------------------------------------------------------
;
; 	FILE NAME  	: TP2_Lib_Timers_UART.H
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configuration pour le TP BSE
;
; 	REVISION 1.0
;
;---------------------------------------------------------------------------*/

#ifndef __TP6_BSE_Lib_Frequencemetre__
#define __TP6_BSE_Lib_Frequencemetre__


void Config_Timer4_Event_Counter(unsigned int);
unsigned long Mesure_frequence(void);

#endif