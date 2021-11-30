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
#include "TP3_BSE_Lib_Divers.h"

//*****************************************************************************	 
//*****************************************************************************	 
// Software_Delay_10ms -- Temporisation 10ms
//  Pour SYSCLK = 2 MHz - Niveau optimisation 0 du compilateur
//  Vérifiée en  simulation
void Software_Delay_10ms(void)
   { 
	 unsigned int i;
	 for(i=0;i<998;i++){}
	 }
	 
//*****************************************************************************	 
//*****************************************************************************	 
// Software_Delay -- Temporisation paramétrable
//      Pour SYSCLK = 2 MHz - Niveau optimisation 0 du compilateur
//      Vérifiée en  simulation
//      L'argument passé en exprimé en centièmes de seconde (dans l'hypothèse 
//      où SYSCLK = 2MHz)
	 
 void Software_Delay(unsigned int hundredth_second)
   { 
	 unsigned int i;
	 
	 for(i=0;i<hundredth_second;i++)
      { 
	    Software_Delay_10ms();
			}
	 }

//*****************************************************************************	 
//*****************************************************************************	 
// Software_Delay_10micro -- Temporisation 10 microsecondes
//  Pour SYSCLK = 2 MHz - Niveau optimisation 0 du compilateur
//  Vérifiée en  simulation
void Software_Delay_10micro(void)
   { 
		// L'instruction assembleur NOP est introduite directement dans le code.
    // 1 NOP "consomme" 1 cycle CPU		 
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();
   _nop_();
   _nop_();		
   _nop_();			 
	 }
