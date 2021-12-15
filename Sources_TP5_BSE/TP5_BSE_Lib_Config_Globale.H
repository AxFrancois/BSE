/*---------------------------------------------------------------------------
;
; 	FILE NAME  	: TP2_Lib_Config_Globale_8051F020.H
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configuration pour le TP BSE
;
; 	REVISION 1.0
;
;---------------------------------------------------------------------------*/

#ifndef __TP2_Lib_Config_Globale_8051F020__
#define __TP2_Lib_Config_Globale_8051F020__


void Init_Device (void);
void Reset_Sources_Init();
void Port_IO_Init();
void Oscillator_Init_Osc_Quartz();

#endif