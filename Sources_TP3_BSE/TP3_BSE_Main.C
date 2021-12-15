//-----------------------------------------------------------------------------
// TP2_BSE.c
//-----------------------------------------------------------------------------
// AUTH: 
// DATE: 
//
// Target: C8051F02x
// Tool chain: KEIL Microvision5
//
//-----------------------------------------------------------------------------
// Fichiers d'entête
#include "intrins.h"
#include<c8051F020.h>
#include<c8051F020_SFR16.h>
#include<TP3_BSE_Lib_Config_Globale.h>
#include<TP3_BSE_Lib_Divers.h>
#include<TP3_BSE_Main.h>

//-----------------------------------------------------------------------------
// Déclaration des MACROS
#define SYSCLK 2000000L
#define LED_ON 1

#define LED_OFF 0
#define LED_BLINK 0
#define BP_ON 0
#define BP_OFF 1
#define TO_BE_PROCESSED 1
#define PROCESSED 0
#define SET_VISU_INT6 P6 |= (1<<4)
#define RESET_VISU_INT6 P6 &= ~(1<<4)
//-----------------------------------------------------------------------------
// Déclarations Registres et Bits de l'espace SFR
sbit LED = P1^6;  // LED
sbit BP =P3^7;
sbit VISU_INT7 = P2^4;
sbit VISU_INT_TIMER2 = P3^5;
sbit SIG_OUT = P1^7;

//-----------------------------------------------------------------------------
// Variable globale
Event = PROCESSED;
ComptageEvenement;
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {
	


 	   // Configurations globales
	      Init_Device();
	      Modif_Cfg_Globale ();
	
	   // Configurations  spécifiques  
	      Config_INT7(); // Configuration de INT7
	      Config_INT6(); // Configuration de INT6
	      Config_Timer2_TimeBase();
				Config_Timer4_TimeBase();

	   // Fin des configurations			
	      EA = 1;  // Validation globale des interruptions
	
// Boucle infinie	
        while(1);
      				               	
			}
//******************************************************************************
void Config_INT7(void)
{
	P3IF &= ~(1<<7); // IE7 mis à 0 pending flag de INT7 effacé
	P3IF &= ~(1<<3); // IE7CF mis à 0 - sensibilité int7 front descendant	
	
	EIP2 &= ~(1<<5);  // PX7 mis à 0 - INT7 priorité basse
	EIE2 |= (1<<5);  // EX7 mis à 1 - INT7 autorisée
}
//******************************************************************************
void ISR_INT7 (void) interrupt 19
{
	VISU_INT7 = 1;
	P3IF &= ~(1<<7); // IE3 mis à 0 - remise à zéro du pending flag de INT7 effacé
	Event = TO_BE_PROCESSED;
	VISU_INT7 = 0;
}	

//*****************************************************************************	 
//******************************************************************************
void Config_INT6(void)
{
	P3IF &= ~(1<<7); // IE6 mis à 0 pending flag de INT6 effacé
	P3IF &= ~(1<<2); // IE6CF mis à 0 - sensibilité int6 front descendant	
	
	EIP2 &= ~(1<<4);  // PX6 mis à 0 - INT7 priorité basse
	EIE2 |= (1<<4);  // EX6 mis à 1 - INT7 autorisée
}

//******************************************************************************
void ISR_INT6 (void) interrupt 18
{
	SET_VISU_INT6;
	P3IF &= ~(1<<6); // IE6 mis à 0 - remise à zéro du pending flag de INT6 effacé
	P3IF ^= (1<<2);   // Action sur IE6CF - Commutation Front montant / Front Descendant
	Event = TO_BE_PROCESSED;
	RESET_VISU_INT6;
}	

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
	//RCAP2 = -((SYSCLK/12)/100);
	RCAP2L = 0x42;
	RCAP2H = 0xbb;
  T2 = RCAP2;
  TR2 = 1;                           // Timer2 démarré
  PT2 = 1;							  // Priorité Timer2 Haute

   ET2 = 1;							  // INT Timer2 autorisée
}

void Config_Timer4_TimeBase(void)
{
                   
//	TF4 = 0;  // RAZ TF4
//	EXF4 = 0;  // RAZ EXF4
//  RCLK1 = 0;         
//  TCLK1 = 0;
//  CPRL4 = 0;  // Mode Auto-Reload	
//	EXEN4 = 1;   // Timer4 external Enable
//  CT4 = 1;    // Mode Counter
	
	RCAP4L = 0xf6;
	RCAP4H = 0xff;
	T4 = RCAP4;
  //TR4 = 1;                // Timer4 démarré
  //PT4 = 0;							  // Priorité Timer4 Haute
	EIP2 |= (1<<2);
	EIE2 |= (1<<2);
  T4CON = 0x06; 
  //ET4 = 1;							  // INT Timer4 autorisée
}
//******************************************************************************
void ISR_Timer2 (void) interrupt 5
{
	static char CP_Cligno;
	static bit STATE_LED = LED_BLINK;
	
	VISU_INT_TIMER2 = 1; // T = 9.548ms, etat_haut = 22.5us
	CP_Cligno++;
	if (CP_Cligno > 11) CP_Cligno = 0;
	if (TF2 == 1)
	{
		TF2 = 0;
		if (Event == TO_BE_PROCESSED)
						 {
							 Event = PROCESSED;
							 STATE_LED =  !STATE_LED;	
						 }
   
    if (STATE_LED == LED_BLINK)
						{
							if (CP_Cligno < 2) LED = LED_ON;
							else LED = LED_OFF;
						}
						else LED = LED_OFF;						
	}
	if (EXF2 == 1)
	{
		EXF2 = 0;
	}
	
	VISU_INT_TIMER2 = 0;
}	

void ISR_Timer4 (void) interrupt 16
{
	
	if ((T4CON & (1<<7)) == 0x80)
	{
		T4CON &= ~(1<<7);
		SIG_OUT = !SIG_OUT;
	}
	if ((T4CON & (1<<6)) == 0x40)
	{
		T4CON &= ~(1<<6);
	}
	
}
//******************************************************************************
void Modif_Cfg_Globale (void)
{
	  // P0.0  -  TX0 (UART0), Push-Pull,  Digital
    // P0.1  -  RX0 (UART0), Push-Pull,  Digital
    // P0.2  -  INT0 (Tmr0), Push-Pull,  Digital
    // P0.3  -  INT1 (Tmr1), Push-Pull,  Digital
    // P0.4  -  T2 (Timer2), Push-Pull,  Digital
    // P0.5  -  T2EX (Tmr2), Push-Pull,  Digital
    // P0.6  -  T4 (Timer4), Open,  Digital
    // P0.7  -  T4EX (Tmr4), Push-Pull,  Digital

    // P1.0  -  SYSCLK,      Push-Pull,  Digital
	  P0MDOUT   = 0xBF;
    P1MDOUT   = 0x01;
		P1MDOUT |= (1<<6);  // P1.6  en Push Pull
    XBR0      = 0x04;
    XBR1      = 0xF4;
    XBR2      = 0x58;
	
	OSCXCN =  0x67;
	while((OSCXCN & (1<<7)) != 0x80)
	{
	}
	OSCICN = (1<<3);
	

	
}	