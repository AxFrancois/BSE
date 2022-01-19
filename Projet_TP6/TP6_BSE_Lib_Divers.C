/******************************************************************************
; 	FILE NAME  	: TP2_BSE_Lib_Divers.C
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: Fonctions diverses mises à disposition
;
; 	REVISION 1.0
;   Ce fichier contient diverses fonctions susceptibles de vous faire gagner 
;   un peu de temps
*/
//******************************************************************************

#include "C8051F020.h"
#include "intrins.h"
#include "c8051F020_SFR16.h"
#include "TP6_Def_Globales.h"
#include "TP6_BSE_Lib_Divers.h"

//*****************************************************************************	 
//*****************************************************************************	 
// Software_Delay_10ms -- Temporisation 10ms
//  Pour SYSCLK = 2 MHz - Niveau optimisation 0 du compilateur
//  Vérifiée en  simulation
//void Software_Delay_10ms(void)
//   { 
//	 unsigned int i;
//	 for(i=0;i<998;i++){}
//	 }
	 
//*****************************************************************************	 
//*****************************************************************************	 
// Software_Delay -- Temporisation paramétrable
//      Pour SYSCLK = 2 MHz - Niveau optimisation 0 du compilateur
//      Vérifiée en  simulation
//      L'argument passé en exprimé en centièmes de seconde (dans l'hypothèse 
//      où SYSCLK = 2MHz)
	 
// void Software_Delay(unsigned int hundredth_second)
//   { 
//	 unsigned int i;
//	 
//	 for(i=0;i<hundredth_second;i++)
//      { 
//	    Software_Delay_10ms();
//			}
//	 }

//*****************************************************************************	 
//*****************************************************************************	 
// Software_Delay_10micro -- Temporisation 10 microsecondes
//  Pour SYSCLK = 2 MHz - Niveau optimisation 0 du compilateur
//  Vérifiée en  simulation
//void Software_Delay_10micro(void)
//   { 
//		// L'instruction assembleur NOP est introduite directement dans le code.
//    // 1 NOP "consomme" 1 cycle CPU		 
//	 _nop_();
//	 _nop_();
//	 _nop_();
//	 _nop_();
//	 _nop_();
//	 _nop_();
//   _nop_();
//   _nop_();		
//   _nop_();			 
//	 }

//*****************************************************************************	 
//*****************************************************************************	 
// Delay_CLK22M_10micro -- Temporisation 10 microsecondes
//  Pour SYSCLK = 22,1184 MHz - Niveau optimisation 0 du compilateur
//  Vérifiée en  simulation
//*****************************************************************************
//void Delay_CLK22M_10micro(void)
//   { 
//unsigned char i;
//     for (i=0;i<10;i++);		 
//	 }
//*****************************************************************************	 
//*****************************************************************************	
//  Disable_Interupt()   - Désactivation des interruptions
//*****************************************************************************	
void Disable_Interrupt(void)
{
	EA = 0;
}	
//*****************************************************************************	 
//*****************************************************************************	
//  Enable_Interupt()   - Activation des interruptions
//*****************************************************************************	
void Enable_Interrupt(void)
{
	EA = 1;
}	
