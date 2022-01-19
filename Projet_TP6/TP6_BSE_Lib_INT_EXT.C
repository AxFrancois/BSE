/******************************************************************************
; 	FILE NAME  	: TP_BSE_Lib_INT_EXT.C
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Configurations et Gestion des Interruption externes
;
; 	REVISION 1.0
;   
*/
//******************************************************************************

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "TP6_Def_Globales.h"
#include "TP6_BSE_Lib_INT_EXT.h"


//******************************************************************************
//******************************************************************************


//******************************************************************************
//void Config_INT7(void)
//{
//	P3IF &= ~(1<<7); // IE7 mis à 0 pending flag de INT7 effacé
//	P3IF &= ~(1<<3); // IE7CF mis à 0 - sensibilité int7 front descendant	
//	
//	EIP2 &= ~(1<<5);  // PX7 mis à 0 - INT7 priorité basse
//	EIE2 |= (1<<5);  // EX7 mis à 1 - INT7 autorisée
//}
//******************************************************************************
//******************************************************************************
//
//******************************************************************************
//void Config_INT6(void)
//{
//	P3IF &= ~(1<<7); // IE6 mis à 0 pending flag de INT6 effacé
//	P3IF &= ~(1<<2); // IE6CF mis à 0 - sensibilité int6 front descendant	
//	
//	EIP2 &= ~(1<<4);  // PX6 mis à 0 - INT7 priorité basse
//	EIE2 |= (1<<4);  // EX6 mis à 1 - INT7 autorisée
//}

///******************************************************************************
//******************************************************************************
// ISR_INT7 () - INTERRUPTION - Detection d'une action sur le Bouton poussoir P3.7
//******************************************************************************
void ISR_INT7 (void) interrupt 19  // Interruption Bouton poussoir
{
	PIN_FLAG_VISU_INT7 = 1;
	P3IF &= ~(1<<7); // IE3 mis à 0 - remise à zéro du pending flag de INT7 effacé
	//	Event = TO_BE_PROCESSED;
	PIN_FLAG_VISU_INT7 = 0;
}	
///******************************************************************************
///******************************************************************************
// ISR_INT6 () - INTERRUPTION - Détection de fronts montants et descendants sur P3.6
//******************************************************************************
void ISR_INT6 (void) interrupt 18 // Interruption externe sur P3.6
{
	PIN_FLAG_VISU_INT6;
	P3IF &= ~(1<<6); // IE6 mis à 0 - remise à zéro du pending flag de INT6 effacé
	P3IF ^= (1<<2);   // Action sur IE6CF - Commutation Front montant / Front Descendant
//	Event = TO_BE_PROCESSED;
	PIN_FLAG_VISU_INT6;
}	

