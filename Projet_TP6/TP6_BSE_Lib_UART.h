/*---------------------------------------------------------------------------
;
; 	FILE NAME  	: TP2_Lib_BSE_UART.H
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configuration pour le TP BSE
;
; 	REVISION 1.0
;
;---------------------------------------------------------------------------*/

#ifndef __TP6_BSE_Lib_UART__
#define __TP6_BSE_Lib_UART__



void CFG_Clock_UART0(void);
void CFG_UART0(void);
char putchar(char c);
char _getkey(void);
char getkey_one_time(void);
char xdata *Test_Data_Received_UART0(void);
void Init_Buf_RX_UART0(void);
char Transmit_to_UART0(char *data_to_send);


#endif