//******************************************************************************
// 	FILE NAME  	: TP5_BSE_Lib_TimeBase.C
// 	TARGET MCUs	: C8051F020, 
// 	DESCRIPTION	: 
//
// 	REVISION 1.0
//  Ressources utilisées:
//                         - Timer2
//                         -  GPIO (LED)
//
//******************************************************************************

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "string.h"
#include "TP6_Def_Globales.h"
#include "TP6_BSE_Lib_TimeBase.h"


//******************************************************************************
//******************************************************************************
// Config Tmer2 - Timebase 10ms
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
//******************************************************************************
//  Base de temps de 10ms
// Gestion du clignotement de la LED
// Prise en compte du changement de mode de clignotement 
//*******************************************************************************
void ISR_Timer2 (void) interrupt 5
{

	static bit STATE_LED = LED_BLINK;

	VISU_INT_TIMER2 = 1;
	
	
	if (TF2 == 1)
	{
		TF2 = 0;
    Gestion_cde_Servo();  // Gestion du servomoteur
		Gestion_LED(); //  Gestion du mode de clignotement de la LED					 
  }
	
	// Sécurité: si EXF2 est à 1 - RAZ de EXF2 	
	if (EXF2 == 1)
	{
		EXF2 = 0;
	}
	
	VISU_INT_TIMER2 = 0;
}	
//******************************************************************************
//******************************************************************************
// Gestion_LED()
//******************************************************************************
#define Time_LED_ON  5 
#define Time_LED_OFF 10

void Gestion_LED(void)
{
static bit STATE_LED = LED_BLINK;
static unsigned char CP_Cligno = 0;
	
    CP_Cligno++;
	  if (CP_Cligno >= (Time_LED_ON+Time_LED_OFF)) CP_Cligno = 0;
    if (STATE_LED == LED_BLINK)
						{
							if (CP_Cligno < Time_LED_ON) LED = LED_ON;
							else LED = LED_OFF;
						}
						else LED = LED_OFF;		
	}
