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
// Fichiers d'ent�te
#include "intrins.h"
#include<c8051F020.h>
#include<c8051F020_SFR16.h>
#include<TP2_BSE_Lib_Config_Globale.h>
#include<TP2_BSE_Lib_Divers.h>
#include<TP2_BSE_Main.h>
//-----------------------------------------------------------------------------
// D�claration des MACROS
#define LED_ON 1
#define LED_OFF 0
#define LED_BLINK 0
#define BP_ON 0
#define BP_OFF 1
#define TO_BE_PROCESSED 1
#define PROCESSED 0
#define SET_VISU_INT7_END P6 |= (1<<0)
#define RESET_VISU_INT7_END P6 &= ~(1<<0)
//-----------------------------------------------------------------------------
// D�clarations Registres et Bits de l'espace SFR
sbit LED = P1^6;  // LED
sbit BP =P3^7;
sbit VISU_INT7_START = P2^0;
//-----------------------------------------------------------------------------
// Variable globale
Event = PROCESSED;

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {
	
bit STATE_LED = LED_BLINK;

 	   // Configurations globales
	      Init_Device();
	   // Configurations  sp�cifiques  
	      Config_INT7(); // Configuration de INT7
	   // Fin des configurations
	      
	      EA = 1;  // Validation globale des interruptions
	
// Boucle infinie	
        while(1)
        {  
					   if (Event == TO_BE_PROCESSED)
						 {
							 Event = PROCESSED;
							 STATE_LED =  !STATE_LED;	
						 }
   
            if (STATE_LED == LED_BLINK)
						{
							LED = LED_ON;
							Software_Delay(2);
					   LED = LED_OFF;
					   Software_Delay(10);
						}
						else LED = LED_OFF;						
        }						               	
			}
//******************************************************************************
void Config_INT7(void)
{
	P3IF &= ~(1<<7); // IE3 mis � 0 pending flag de INT7 effac�
	P3IF &= ~(1<<3); // IE7CF mis � 0 - sensibilit� int7 front descendant	
	
	EIP2 &= ~(1<<5);  // PX7 mis � 0 - INT7 priorit� basse
	EIE2 |= (1<<5);  // EX7 mis � 1 - INT7 autoris�e
}
//******************************************************************************
void ISR_INT7 (void) interrupt 19
{
	P3IF &= ~(1<<7); // IE3 mis � 0 - remise � z�ro du pending flag de INT7 effac�
	Event = TO_BE_PROCESSED;
}	

//*****************************************************************************	 
