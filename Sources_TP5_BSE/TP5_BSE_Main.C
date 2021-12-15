//-----------------------------------------------------------------------------
// TP6_BSE_Main.c
// AUTH: FJ
// DATE: 5/12/21
//
// Target: C8051F02x
// Tool chain: KEIL Microvision5
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Cette application exécute les tâches suivantes:
// SYSCLK = Quartz externe = 22.1184 MHz (Visu sur P1.0)
// Clignotement de la LED P1.6
// Commande du clignotement de la LED avec le bouton poussoir P3.7 (INT7) et 
// via le signal DECL_EXT sur P3.6 (INT6)

// Mise en oeuvre de la base de temps Timer2 de 10ms et interruption Timer2
// Mise en oeuvre de l'UART0 à 115200 Bd, pas de parité, 8 bits, 1 stop bit 
 
// Visu Flag INT7 sur P2.4
// Visu Flag INT6 sur P6.4
// Visu Flag INT Timer2 sur P3.5

//*****************************************************************************
//-----------------------------------------------------------------------------
// Fichiers d'entête
#include "intrins.h"
#include<stdio.h>
#include<stdlib.h>
#include<c8051F020.h>
#include<c8051F020_SFR16.h>
#include<TP5_BSE_Lib_Config_Globale.h>
#include<TP5_BSE_Lib_Divers.h>
#include<TP5_BSE_Lib_Timers_UART.h>
#include<TP5_BSE_Main.h>

//-----------------------------------------------------------------------------
// Déclaration des MACROS


#define LED_ON 1

#define LED_OFF 0
#define LED_BLINK 0
//#define BP_ON 0
//#define BP_OFF 1
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
//sbit VISU_INT_TIMER4 = P3^4;
//sbit SIG_OUT = P3^3;
//-----------------------------------------------------------------------------
// Variables globales

bit Event = PROCESSED;  // Détection des évènements pour changer le clignotement de la LED
//unsigned int Event_to_Count = 100 ; //Valeur du comptage d'évènements 
//long frequence = 0;  // Fréquence calculée de SIG_IN
//unsigned int CP_Overflow_Timer4; // Compteur d'overflows du Timer4
bit 	Flag_Seconde = 0; // drapeau 1s écoulée
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {
	

unsigned char temp_char;
char xdata buf[20];
char i;
int value;
	
 	   // Configurations globales
	      Init_Device();
	     
	   // Configurations  spécifiques  
	      Config_INT7(); // Configuration de INT7
	      Config_INT6(); // Configuration de INT6
	      Config_Timer2_TimeBase();
	      CFG_Clock_UART0();
	      CFG_UART0();
	      Voltage_Reference_Init();
	      ADC0_Init_Etape2();
//      ADC0_Init_Etape4();	
	      DAC0_Init();
// Fin des configurations	
	       printf("\n\r**** TP5 - System OK ****\n\n\r");
	      EA = 1;  // Validation globale des interruptions
	
// Boucle infinie	
         while(1)
         {
					 // Code à ajouter...
				 }
 	
         					 
      				               	
}
//******************************************************************************
// Voltage_Reference_Init
//******************************************************************************
void  Voltage_Reference_Init(void)
{
	// Code de configuration de Vref à insérer
}

//******************************************************************************
// ADC0_Init_Etape2
//******************************************************************************
void  ADC0_Init_Etape2(void)
{
	// Code de configuration de l'ADC0 pour l'étape2 à insérer
}

//******************************************************************************
// ADC0_Init_Etape4
//******************************************************************************
void  ADC0_Init_Etape4(void)
{
	// Code de configuration de l'ADC0 pour l'étape4 à insérer
}
//******************************************************************************
// DAC0_Init
//******************************************************************************
void  DAC0_Init(void)
{
	// Code de configuration de l'ADC0 pour l'étape4 à insérer
}

//******************************************************************************
// Timer3_Init
//******************************************************************************
void  Timer3_Init_Fech(void)
{
	// Code de configuration du Timer3
}
//******************************************************************************
// Detection d'une action sur le Bouton poussoir P3.7
//******************************************************************************
void ISR_INT7 (void) interrupt 19  // Interruption Bouton poussoir
{
	VISU_INT7 = 1;
	P3IF &= ~(1<<7); // IE3 mis à 0 - remise à zéro du pending flag de INT7 effacé
	Event = TO_BE_PROCESSED;
	VISU_INT7 = 0;
}	

//******************************************************************************
// Détection de fronts montants et descendants sur P3.6
//******************************************************************************
void ISR_INT6 (void) interrupt 18 // Interruption externe sur P3.6
{
	SET_VISU_INT6;
	P3IF &= ~(1<<6); // IE6 mis à 0 - remise à zéro du pending flag de INT6 effacé
	P3IF ^= (1<<2);   // Action sur IE6CF - Commutation Front montant / Front Descendant
	Event = TO_BE_PROCESSED;
	RESET_VISU_INT6;
}	


//******************************************************************************
//  Base de temps de 10ms
// Gestion du clignotement de la LED
// Prise en compte du changement de mode de clignotement 
//*******************************************************************************
void ISR_Timer2 (void) interrupt 5
{
	static char CP_Cligno;
	static char CP_Seconde = 0;
	static bit STATE_LED = LED_BLINK;
	char Read_Cfg_Event;
	static unsigned int CP_Timer4 = 0;
	static unsigned int OLD_CP_Timer4 = 0;

	VISU_INT_TIMER2 = 1;
	
	
	if (TF2 == 1)
	{
		TF2 = 0;
	  CP_Seconde++;
	  if (CP_Seconde >= 100) 
	  {
		  CP_Seconde = 0;
		  Flag_Seconde = 1;
	  }	
	// Gestion des évènements INT6 et INT7
	// pour gérer les modes de clignotement de la LED
		if (Event == TO_BE_PROCESSED)
						 {
							 Event = PROCESSED;
							 STATE_LED =  !STATE_LED;	
						 }
	// Gestion du mode de clignotement de la LED					 
    CP_Cligno++;
	  if (CP_Cligno > 11) CP_Cligno = 0;
    if (STATE_LED == LED_BLINK)
						{
							if (CP_Cligno < 2) LED = LED_ON;
							else LED = LED_OFF;
						}
						else LED = LED_OFF;		
	}
	// Sécurité: si EXF2 est à 1 - RAZ de EXF2 	
	if (EXF2 == 1)
	{
		EXF2 = 0;
	}
	
	VISU_INT_TIMER2 = 0;
}	


