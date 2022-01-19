//******************************************************************************
// 	FILE NAME  	: TP5_BSE_Frequencemetre.C
// 	TARGET MCUs	: C8051F020, 
// 	DESCRIPTION	: 
//
// 	REVISION 1.0
//  Ressources utilisées:
//                         - Timer4
//   Fonctionnement du Timer en mode capture - Entrée du signal SIG_IN sur T4EX
//******************************************************************************/******************************************************************************

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "string.h"
#include "TP6_Def_Globales.h"
#include "TP6_BSE_Lib_Frequencemetre.h"	


unsigned long CP_CLK_Timer=0;


//******************************************************************************
void Config_Timer4_Frequency_Meter (void)
{
	// Timer 4 configuré en mode Timer - Capture
	// pour une application fréquencemètre
	// Timer4 en mode capture - entrée sur T4EX
	// Mode Timer
	// CLKTimer = SYSCLK
	
	CKCON |= (1<<6);   // Clk Timer = SYSCLK
	T4CON = 0x09;   // Flags TF4 et EXF4 effacés
	                // RCLK1 et TCLK1 à zéro
	                // Transitions sur T4EX autorisés
	                // Timer stoppé
	                // Timer en mode Timer
	                // Mode Capture
  T4 = 0;
  T4CON |= (1<<2);      // TR4 = 1 -- Timer2 démarré
  EIP2 |= (1<<2);      //PT4 = 0 -- Priorité Timer4 haute
  EIE2 |= (1<<2);      //ET4 = 1 --  INT Timer4 autorisée
}
//******************************************************************************
void ISR_Timer4 (void) interrupt 16  // Etape4
{
	

static unsigned int Old_Capture_Value = 0;
static unsigned int Capture_Value = 0;
static unsigned int CP_Overflow_T4;
 
	
	VISU_INT_TIMER4 = 1;
	//CP_Cligno++;
	//if (CP_Cligno > 11) CP_Cligno = 0;
	if ((T4CON & (1<<7)) != 0)   // Test Overflow
	{
		T4CON &= ~(1<<7);    // RAZ TF4
		CP_Overflow_T4++;
		if (CP_Overflow_T4 == 0xFFFF) 
		{	
			 CP_CLK_Timer = 0;
		   CP_Overflow_T4--;
		}
		
	}
	if ((T4CON & (1<<6)) != 0)   // test Capture
	{
		T4CON &= ~(1<<6);  // RAZ EXF4
		Capture_Value = RCAP4;
		CP_CLK_Timer= ((CP_Overflow_T4 * 65536L)+Capture_Value - Old_Capture_Value);
		Old_Capture_Value = Capture_Value;
		CP_Overflow_T4 = 0;
					
	}
	
	VISU_INT_TIMER4 = 0;
}	

// ***************************************************************************
//
//*****************************************************************************
long Calcul_frequence_mHz(long CP_CLK_Timer)
{
	if (CP_CLK_Timer == 0) return 0;		
	else return ((SYSCLK*50)/CP_CLK_Timer)*20;  // pour exprimer la valeur en millihetz
}
// ***************************************************************************
//
//*****************************************************************************
unsigned long Mesure_frequence(void)
{
	unsigned long Value;
	
	Disable_Interrupt();
	Value = CP_CLK_Timer;
	Enable_Interrupt();
	return Calcul_frequence_mHz(Value);
	
}


