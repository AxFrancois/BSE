/*---------------------------------------------------------------------------
;
; 	FILE NAME  	: TP6_Lib_ADC-DAC-VREF.H
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configuration pour le TP BSE
;
; 	REVISION 1.0
;
;---------------------------------------------------------------------------*/

#ifndef __TP6_Lib_ADC_DAC-VREF__
#define __TP6_Lib_ADC_DAC-VREF__



void  Timer3_Init_Fech_ADC0(void);
void  Voltage_Reference_Init(void);

void  ADC0_Init_Decl_T3(void);
void  DAC0_Init(void);
void  Set_Seuils_ADC(unsigned int,unsigned int);