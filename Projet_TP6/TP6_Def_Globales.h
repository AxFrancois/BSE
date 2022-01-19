/*---------------------------------------------------------------------------
;
; 	FILE NAME  	: TP6_Def_Globales.H
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configuration pour le TP BSE
;
; 	REVISION 1.0
;
;---------------------------------------------------------------------------*/

#ifndef __TP6_Def_Globales__
#define __TP6_Def_Globales__


#define LED_ON 1
#define LED_OFF 0
#define LED_BLINK 0

#define BP_ON 0
#define BP_OFF 1

#define TO_BE_PROCESSED 1
#define PROCESSED 0

#define SYSCLK 22118400L
#define BAUDRATE 115200L

#define YES 1
#define NO 0

#define ON 1
#define OFF 0

sbit LED = P1^6;  // LED
sbit BP =P3^7;
// Sortie Servomoteur
sbit PIN_Cde_Servo = P2^4;
// Flags interruption
sbit PIN_FLAG_VISU_INT7 = P3^0;
sbit PIN_FLAG_VISU_INT6 = P3^1;
sbit VISU_INT_TIMER4 = P3^2;
sbit VISU_INT_UART0 = P3^3;
sbit VISU_INT_ADC0 = P3^4;
sbit VISU_INT_TIMER2 = P3^5;

#endif