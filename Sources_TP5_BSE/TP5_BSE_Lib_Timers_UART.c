/******************************************************************************
; 	FILE NAME  	: TP5_BSE_Lib_Timers_UART.C
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configurations des Timers et des UART
;
; 	REVISION 1.0
;   
*/
//******************************************************************************

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "TP5_BSE_Lib_Timers_UART.h"


//*****************************************************************************	 
//******************************************************************************
void Config_Timer2_TimeBase(void)
{
	CKCON &= ~(1<<5);         // T2M: Timer 2 Clock Select
                         // CLK = sysclk/12TR2 = 0;  //Stop Timer
	TF2 = 0;  // RAZ TF2
	EXF2 = 0;  // RAZ EXF2
  RCLK0 = 0;         
  TCLK0 = 0;
  CPRL2 = 0;  // Mode AutoReload	
	EXEN2 = 0;   // Timer2 external Enable Disabled 
  CT2 = 0;    // Mode Timer

	RCAP2 = -((SYSCLK/12)/100);
  T2 = RCAP2;
  TR2 = 1;                           // Timer2 démarré
  PT2 = 1;							  // Priorité Timer2 Haute

   ET2 = 1;							  // INT Timer2 autorisée
}

//******************************************************************************
void Config_Timer4_Event_Counter(unsigned int Event_to_Count) // Pour étape 4
{
	// Timer 4 configuré en compteur d'évènements SIG_IN
	// SIG_IN câblé sur l'entrée T4 du Timer4
	
	// Timer4 en mode autorechargement
	// Mode Counter
	// CLKTimer = SIG_IN
	// Valeur rechargement= (65536 - Nbre_Event_to_count)
	// L'overflow, et donc l'interruption Timer4 signale le comptage de 100 évènements
	T4CON = 0x02;   // Flags TF4 et EXF4 effacés
	                // RCLK1 et TCLK1 à zéro
	                // Transitions sur T4EX ignorées
	                // Timer stoppé
	                // Timer en mode counter
	                // Mode auto-reload
  RCAP4 = 65536-Event_to_Count;
  T4 = RCAP4;
  T4CON |= (1<<2);      // TR4 = 1 -- Timer2 démarré
  EIP2 |= (1<<2);      //PT4 = 0 -- Priorité Timer4 haute
  EIE2 |= (1<<2);      //ET4 = 1 --  INT Timer4 autorisée
}

//*****************************************************************************	 
//CFG_clock_UART
//	Utilisation du Timer 1
//
// 
//*****************************************************************************	 
void CFG_Clock_UART0(void)
{
    CKCON |= (1<<4);             // T1M: Timer 1 Clock Select
	                               // Timer CLK = SYSCLK
	
    TMOD |= (1<<5);             
	  TMOD &= ~((1<<7)|(1<<6)|(1<<4));			  // Timer1 configuré en Timer 8 bit avec auto-reload	
	  TF1 = 0;				  // Flag Timer effacé
    TH1 = -(Preload_Timer1);
	  ET1 = 0;				   // Interruption Timer 1 dévalidée
	  TR1 = 1;				   // Timer1 démarré
}
 
//*****************************************************************************	 
//CFG_uart0_mode1
//
//
// 
//*****************************************************************************	 
void CFG_UART0(void)
{
		RCLK0 = 0;   // Source clock Timer 1
		TCLK0 = 0;
		PCON  |= (1<<7); //SMOD0: UART0 Baud Rate Doubler Disabled.
		PCON &= ~(1<<6); // SSTAT0=0
		SCON0 = 0x70;   // Mode 1 - Check Stop bit - Reception validée
		               
 //   TI0 = 0;     // Drapeaux TI et RI à zéro
    RI0 = 0;	
	  TI0 = 1;  //à cause du codage de putchar
	
	  ES0 = 0;  // Interruption UART autorisée
}
//*****************************************************************************	 
char putchar(char c)
{
	while(TI0==0);
	TI0 = 0;
	SBUF0 = c;
	return c;
}

//*****************************************************************************	 
char _getkey(void)
{
  char c;
	while(RI0==0);
	RI0 = 0;
	c = SBUF0;
	return c;
	
}

//*****************************************************************************	 
char getkey_one_time(void)
	
{
  char c;
	if (RI0==0) return 0;
	else
  {
	   RI0 = 0;
	   return SBUF0;
	}
}


