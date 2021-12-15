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
#include<TP2_BSE_Lib_Config_Globale.h>
#include<TP2_BSE_Lib_Divers.h>
#include<TP2_BSE_Main.h>
//-----------------------------------------------------------------------------
// Déclaration des MACROS
#define LED_ON 1
#define LED_OFF 0
#define LED_BLINK 0
#define BP_ON 0
#define BP_OFF 1
#define TO_BE_PROCESSED 1
#define PROCESSED 0
#define SET_VISU_INT6_END P6 |= (1<<4)
#define RESET_VISU_INT6_END P6 &= ~(1<<4)

//-----------------------------------------------------------------------------
// Déclarations Registres et Bits de l'espace SFR
sbit LED = P1^6;  // LED
sbit BP =P3^7;
sbit VISU_INT7_START = P2^4;
sbit VISU_INT_TIMER2 = P3^5;
sbit PROC_USED = P3^2;
//-----------------------------------------------------------------------------
// Variable globale
Event = PROCESSED;
bit STATE_LED = LED_BLINK;


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {
			//Initialisation
			VISU_INT7_START = 0;
			RESET_VISU_INT6_END;
			VISU_INT_TIMER2 =0;
 	   // Configurations globales
	      Init_Device();
	   // Configurations  spécifiques  
	      Config_INT7(); // Configuration de INT7

				Config_INT6(); // Configuration de INT6
				
				Config_TIMER2(); //Configuration de Timer2
				
	   // Fin des configurations
	      
	      EA = 1;  // Validation globale des interruptions

	
// Boucle infinie	
        while(1){
				}
}
//******************************************************************************
void Config_INT7(void)
{
	P3IF &= ~(1<<7); // IE3 mis à 0 pending flag de INT7 effacé
	P3IF &= ~(1<<3); // IE7CF mis à 0 - sensibilité int7 front descendant	
	
	EIP2 &= ~(1<<5);  // PX7 mis à 0 - INT7 priorité basse
	EIE2 |= (1<<5);  // EX7 mis à 1 - INT7 autorisée
}
//******************************************************************************
void ISR_INT7 (void) interrupt 19
{
	VISU_INT7_START = 1;
	PROC_USED = 1;
	P3IF &= ~(1<<7); // IE3 mis à 0 - remise à zéro du pending flag de INT7 effacé
	Event = TO_BE_PROCESSED;
	Software_Delay(5);
	VISU_INT7_START = 0;
	PROC_USED = (P6 &= (1<<4)) || VISU_INT_TIMER2;
}	
void Config_INT6(void)
{
	P3IF &= ~(1<<6); // IE3 mis à 0 pending flag de INT6 effacé
	P3IF |= (1<<2); // IE6CF mis à 0 - sensibilité int6 front montant	

	EIP2 &= ~(1<<4);  // PX6 mis à 0 - INT6 priorité basse
	EIE2 |= (1<<4);  // EX6 mis à 1 - INT6 autorisée
}
void ISR_INT6 (void) interrupt 18
{
	SET_VISU_INT6_END;
	PROC_USED = 1;
	P3IF ^= (1<<2);
	P3IF &= ~(1<<6); // IE3 mis à 0 - remise à zéro du pending flag de INT6 effacé
	Event = TO_BE_PROCESSED;
	Software_Delay(5);
  RESET_VISU_INT6_END;
	PROC_USED = VISU_INT7_START || VISU_INT_TIMER2;
}	

void Config_TIMER2(void)
{
	TF2 = 0;
	T2CON = 0;
	T2CON |= (1<<2); //TR2 timer enable
	CKCON &= ~(1<<5); // T2M = 0 => SYSCLK/12
	ET2 = 1; //IE |= (1<<5) 
	PT2 = 1; // IP |= (1<<5) 
	RCAP2L = 0xee;
	RCAP2H = 0xb7;
}

void ISR_Timer2 (void) interrupt 5
{
	VISU_INT_TIMER2 = 1;
	PROC_USED = 1;
	TF2 = 0;
	
	if (Event == TO_BE_PROCESSED){
		Event = PROCESSED;
		STATE_LED =  !STATE_LED;	
	}
   
  if (STATE_LED == LED_BLINK){
		if (LED == LED_OFF){
			LED = LED_ON;
		}
		else if(LED == LED_OFF){
			LED = LED_ON;
		}
	}
	else LED = LED_OFF;						
	
	Software_Delay(5);
  VISU_INT_TIMER2 = 0;
	PROC_USED = VISU_INT7_START || (P6 &= (1<<4)) ;
}	

//*****************************************************************************	 
