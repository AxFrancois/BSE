/******************************************************************************
; 	FILE NAME  	: TP4_Lib_Config_Globale_8051F020.C
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
#include "TP1_BSE_Lib_Config_Globale.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function

//-----------------------------------------------------------------------------
// Dévalidation du watchdog
//-----------------------------------------------------------------------------
void Reset_Sources_Init()
{
	// La configuration des registres WDTCN  sera étudiée plus tard
	 WDTCN = 0xDE;
	 WDTCN = 0XAD;
}

//-----------------------------------------------------------------------------
// Configuration des Ports d'entrée-sorties
//-----------------------------------------------------------------------------

void Port_IO_Init()
{
    // P0.0  -  Congiguration PIO - TX0 (UART0)
    // P0.1  -  Congiguration PIO - RX0 (UART0)
    // P0.2  -  Congiguration PIO - INT0 (Tmr0)
    // P0.3  -  Congiguration PIO - INT1 (Tmr1)
    // P0.4  to P7.7 - Mode GPIO par défaut 
 
// La configuration des registres XBR sera étudiée plus tard
    XBR0      = 0x04; // Configuration Hors du cadre TP1-BSE
    XBR1      = 0x14; // Configuration Hors du cadre TP1-BSE
    XBR2      = 0x40; // Configuration Hors du cadre TP1-BSE


	  P1MDOUT |= (1<<6);  // P1.6  en Push Pull
		P3MDOUT &= (0<<7);  // P3.7  en drain ouvert
		P3 |= (1<<7);
// La configuration qui suit n'a qu'un intêret pédagogique...	
	 //P3 = 0;
	  //P3MDOUT = 0xFF;
// Fin de la config à intêret pédagogique
}



//-----------------------------------------------------------------------------
// Initialisation globale du Microcontrôleur - 
//-----------------------------------------------------------------------------
void Init_Device(void)
{
    Reset_Sources_Init();
    Port_IO_Init();
}



