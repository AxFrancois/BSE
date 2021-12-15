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


//void Config_INT7 (void);
//void Config_INT6 (void);
//void Config_Timer2_TimeBase(void);
//void Modif_Cfg_Globale (void);
//void Oscillator_Init();
//void Config_Timer4_Event_Counter(unsigned int);
//void CFG_Clock_UART0(void);
//void CFG_UART0(void);
//char getkey_one_time(void);
void  Voltage_Reference_Init(void);
void  ADC0_Init_Etape2(void);
void  ADC0_Init_Etape4(void);
void  DAC0_Init(void);
void  Timer3_Init_Fech(void);

#endif