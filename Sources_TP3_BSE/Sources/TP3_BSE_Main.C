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
//-----------------------------------------------------------------------------
// Variable globale
Event = PROCESSED;

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {
	


 	   // Configurations globales
	      Init_Device();
	   // Configurations  spécifiques  
	      Config_INT7(); // Configuration de INT7
	      Config_INT6(); // Configuration de INT6
	      Config_Timer2_TimeBase();
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
	RCAP2 = -((SYSCLK/12)/100);
  T2 = RCAP2;
  TR2 = 1;                           // Timer2 démarré
  PT2 = 1;							  // Priorité Timer2 Haute

   ET2 = 1;							  // INT Timer2 autorisée
}

//******************************************************************************
void ISR_Timer2 (void) interrupt 5
{
	static char CP_Cligno;
	static bit STATE_LED = LED_BLINK;
	
	VISU_INT_TIMER2 = 1;
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