/*---------------------------------------------------------------------------
;
;
;
;
; 	FILE NAME  	: TP2_Lib_Divers.H
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: 
;
; 	REVISION 1.0
;
;---------------------------------------------------------------------------*/

#ifndef __TP6_Lib_Divers__
#define __TP6_Lib_Divers__


void Software_Delay_10ms(void);
void Software_Delay(unsigned int hundredth_second);
void Software_Delay_10micro(void);
void Delay_CLK22M_10micro(void);
void Disable_Interrupt(void);
void Enable_Interrupt(void);

#endif