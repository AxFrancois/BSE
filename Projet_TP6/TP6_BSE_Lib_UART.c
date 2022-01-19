//******************************************************************************
// 	FILE NAME  	: TP6_BSE_Lib_UART.C
// 	TARGET MCUs	: C8051F020, 
// 	DESCRIPTION	: 
//
// 	REVISION 1.0
//  Ressources utilis�es:
//                         - UART0
//                         - Timer1
//
//******************************************************************************

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "string.h"
#include "TP6_Def_Globales.h"
#include "TP6_BSE_Lib_UART.h"

#define Freq_timer1 (BAUDRATE*16)

#define Preload_Timer1 (SYSCLK/Freq_timer1)
#if Preload_Timer1 > 255 
#error "Valeur Preload Timer1 HORS SPECIFICATIONS"
#endif 

#define Size_Buf_IN 10
#define Size_Buf_OUT 50

typedef struct
{
	char  *start;  // pointeur sur le premier �l�ment du buffer
	char  *end;  // pointeur sur le dernier �l�ment du buffer
	char  *current; // pointeur sur le dernier �l�ment occup� du buffer
} UART_Buf;	

static char xdata UART0_Buf_IN[Size_Buf_IN]; // Buffer de r�ception

static xdata UART_Buf UART_Receive;
static xdata UART_Buf UART_Transmit;

static bit Flag_String_Received = NO;

static bit Flag_TX_in_Progress = NO;

//*****************************************************************************	
//*****************************************************************************	 
//CFG_clock_UART
//	Utilisation du Timer 1 comme source d'horloge UART0
//
// 
//*****************************************************************************	 
void CFG_Clock_UART0(void)
{
    CKCON |= (1<<4);             // T1M: Timer 1 Clock Select
	                               // Timer CLK = SYSCLK
	
    TMOD |= (1<<5);             
	  TMOD &= ~((1<<7)|(1<<6)|(1<<4)); // T1 configur� en Timer 8 bit avec auto-reload	
	  TF1 = 0;				  // Flag Timer effac�
    TH1 = -(Preload_Timer1);
	  ET1 = 0;				   // Interruption Timer 1 d�valid�e
	  TR1 = 1;				   // Timer1 d�marr�
}
//*****************************************************************************	
//*****************************************************************************	 
//CFG_uart0_mode1 -- Configuration de l'UART0
//                   Mode Async - 8 bits
//                   GESTION par INTERRUPTION
// 
//*****************************************************************************	 
void CFG_UART0(void)
{
		RCLK0 = 0;   // Source clock Timer 1
		TCLK0 = 0;
		PCON  |= (1<<7); //SMOD0: UART0 Baud Rate Doubler Disabled.
		PCON &= ~(1<<6); // SSTAT0=0
		SCON0 = 0x70;   // Mode 1 - Check Stop bit - Reception valid�e
		               
   // Drapeaux TI et RI � z�ro
    RI0 = 0;	
	  TI0 = 0;  
	
	  ES0 = 1;  // Interruption UART autoris�e
	  PS = 0;  // Priorit� basse
}
//*****************************************************************************	
//*****************************************************************************	 
//char putchar(char c)
//{
//	while(TI0==0);
//	TI0 = 0;
//	SBUF0 = c;
//	return c;
//}
//*****************************************************************************	
//*****************************************************************************	 
//char _getkey(void)
//{
//  char c;
//	while(RI0==0);
//	RI0 = 0;
//	c = SBUF0;
//	return c;
//	
//}
//*****************************************************************************	
//*****************************************************************************	 
//char getkey_one_time(void)
//	
//{
//  char c;
//	if (RI0==0) return 0;
//	else
//  {
//	   RI0 = 0;
//	   return SBUF0;
//	}
//}
//*****************************************************************************	
//******************************************************************************
// Init_Buf_UART0();  -- Initialisation du buffer de r�ception
//******************************************************************************
void Init_Buf_RX_UART0(void)
{
	UART_Receive.start = UART0_Buf_IN;
	UART_Receive.end = UART0_Buf_IN + (Size_Buf_IN-1);
	UART_Receive.current = UART_Receive.start;
}	
//*****************************************************************************	
//******************************************************************************
// Test_Data_Received_UART0();  Fonction de r�ceptio nde donn�es UART
//  Renvoit un pointeur sur une chaine de caract�res apr�s r�ception de la chaine
//  ou un pointeur NULL si la chaine est en cours de r�ception
//******************************************************************************
char xdata *Test_Data_Received_UART0(void)
{
	 if (Flag_String_Received == YES) 
	 {
		 Flag_String_Received = NO;
		 return UART_Receive.start;
	 } 
	 else return NULL;		 
}	
//*****************************************************************************	
//******************************************************************************
// Transmit_to_UART0();  Fonction de transmission de donn�es uart
//         Passage en param�tre d'un pointeur sur une chaine de caract�res
//         Retourne 0 si la chaine n'a pas pu �tre trait�e (transmission en cours
//         d'une autre chaine) ou retourne le nombre de caract�res qui va �tre transmis.
//******************************************************************************
char Transmit_to_UART0(char *data_to_send)
{
	   if (Flag_TX_in_Progress == NO)
		 {
			 UART_Transmit.start = data_to_send;
	     UART_Transmit.end = data_to_send + strlen(data_to_send);
	     UART_Transmit.current = data_to_send;
	    // Flag_String_Transmitted = NO; 
	     Flag_TX_in_Progress = YES;
		   TI0 =1;
			 return(strlen(data_to_send));
		 }
		 else return 0;
		 
}	
//*****************************************************************************	
//******************************************************************************
//  ISR_UART0  -- Interruption UART0 
//                Gestion Emission et r�ception de caract�res
//                Traitement par chaines de caract�res
//   --  R�ception:  -- stockage dans un buffer 
//           SI la chaine pr�c�demment stock�e dans ce buffer a �t� trait�e  
//           SI il reste de la place dans le buffer
//                   -- D�tection des caract�res Retour Charriot ou Fin de ligne
//                      pour consid�rer que la chaine est termin�e
//   --  Transmission: r�cup�ration des caract�res � transmettre dans un buffer
//                     de transmission jusqu'au caract�re de fin de chaine
//
//*******************************************************************************
void ISR_UART0 (void) interrupt 4
{
	
// static char test; 
	VISU_INT_UART0 = 1;
	
	
	if (RI0 == 1)  // Gestion r�ception de caract�re
	{
		RI0 = 0;
		if (Flag_String_Received == NO)
		{
			if (UART_Receive.current == UART_Receive.end)
			{
				*UART_Receive.current = 0; 
				UART_Receive.current = UART_Receive.start;
				Flag_String_Received = YES;
			}	 
			else
			{
				*UART_Receive.current = SBUF0;
				if ((*UART_Receive.current == 0x0A) || (*UART_Receive.current == 0x0D))
				{ 
					*UART_Receive.current = 0; 
				  UART_Receive.current = UART_Receive.start;
				  Flag_String_Received = YES;
        }					
				else UART_Receive.current++;
			}				
    }			
	}
	if (TI0 == 1)  // // Gestion �mission de caract�re
	{
		TI0 = 0;
		if (Flag_TX_in_Progress == YES)
		{		
		  if (*UART_Transmit.current != 0)
		  {
			   SBUF0 = *UART_Transmit.current;
			   UART_Transmit.current++;
		  }	
		  else 
			{
				Flag_TX_in_Progress = NO;
			}	
		}	
	}
	
	VISU_INT_UART0 = 0;
}	
//*****************************************************************************	
