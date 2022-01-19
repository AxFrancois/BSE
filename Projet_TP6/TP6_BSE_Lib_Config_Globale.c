/******************************************************************************
; 	FILE NAME  	: TP_BSE_Lib_Config_Globale.C
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configurations GLOBALES pour le 8051F020
;
; 	REVISION 1.0
;   Ce fichier contient les codes de configuration globale du 8051F020
;     - Configuration de l'horloge système SYSCLK
;     - Configuration des broches du 80851F020
;     - Configuration Reset et watchdog
*/
//******************************************************************************

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "TP6_Def_Globales.h"
#include "TP6_BSE_Lib_Config_Globale.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function

//*****************************************************************************	 
//*****************************************************************************	 
// Reset_Sources_Init()  - Dévalidation du watchdog
//*****************************************************************************	 
void Reset_Sources_Init()
{
	// La configuration des registres WDTCN  sera étudiée plus tard
	 WDTCN = 0xDE;
	 WDTCN = 0XAD;
}

//*****************************************************************************	 
//*****************************************************************************	 
// Port_IO_Init() - Configuration des Ports d'entrée-sorties
//*****************************************************************************	 
void Port_IO_Init()
{
    // P0.0  -  TX0 (UART0)
    // P0.1  -  RX0 (UART0)
    // P0.2  -  INT0 (Tmr0)
    // P0.3  -  INT1 (Tmr1)
    // P0.4  -  T2 (Timer2)
    // P0.5  -  T2EX (Tmr2)
    // P0.6  -  T4 (Timer4)
    // P0.7  -  T4EX (Tmr4)
    // P1.0  -  SYSCLK

    // P1.2  to P7.7 - Mode GPIO par défaut 
 
    XBR0      = 0x04; 
    XBR1      = 0xF4; 
    XBR2      = 0x58; 
	
// Config pour TX0 de UART0 - P0.0
	  P0MDOUT |= (1<<0);  // P0.0  en Push Pull
// Config pour LED
	  P1MDOUT |= (1<<6);  // P1.6  en Push Pull
// Config sortie SYSCLK
	      P1MDOUT |= (1<<0);  // P1.0 en PP pour SYSCLOCK
				
// Config pour drapeaux matériels d'interruption				
// Gestion des flags interruption de p3.0 à P3.6
	   P3MDOUT = 0x7F;  // P3.0 à P3.6 en Push Pull
	   P3 = 0x80;       // Tous les flags mis à zéro
		 
// Config pour gestion bouton poussoir
	     P3 |= (1<<7); // Mise à 1 de P3.7
       P3MDOUT &= ~(1<<7); // P3.7 en Drain ouvert

// Cde Servomoteur P2.4
        P2MDOUT |= (1<<4);  // P2.4 en Push Pull
				P2 &= ~(1<<4);       // P2.4 mis à zéro

}
//*****************************************************************************	 
//******************************************************************************
//  Oscillator_Init_Osc_Quartz() - Configuration source d'horloge Quartz pour 
//                                 SYSCLK
//*****************************************************************************	 
void Oscillator_Init_Osc_Quartz(void)
{
    int i = 0;
    OSCXCN    = 0x67;
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);
    OSCICN    = 0x08;
}


//*****************************************************************************	 
// Initialisation globale du Microcontrôleur - 
//*****************************************************************************	 
void Init_Device(void)
{
    Reset_Sources_Init();
    Port_IO_Init();
	  Oscillator_Init_Osc_Quartz();
}



