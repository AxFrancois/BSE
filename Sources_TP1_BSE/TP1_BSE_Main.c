//-----------------------------------------------------------------------------
// TP1_BSE.c
//-----------------------------------------------------------------------------
// AUTH: 
// DATE: 
//
// Target: C8051F02x
// Tool chain: KEIL Microvision5
//
//-----------------------------------------------------------------------------
// Déclarations Registres et Bits de l'espace SFR
#include "intrins.h"
#include<c8051F020.h>
#include<c8051F020_SFR16.h>
#include<TP1_BSE_Lib_Config_Globale.h>
#include<TP1_BSE_Lib_Divers.h>

#define LED_ON 1
#define LED_OFF 0
#define BP_OFF 1
#define BP_ON 0

sbit LED = P1^6;  // LED
sbit BP = P3^7; //Bouton poussoir
sbit VISU_INT7_START = P2^0;
sbit VISU_INT7_END = P6^0;
sbit VISU_INT7_WIDTH = P6^1;
int ACK_BP = BP_OFF;

typedef enum{
	clignotement,
	eteinte
}State;
State ETAT_LED = clignotement;
//------------------------------------------------------------------------------------
// Function Prototypes

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void IE7_ISR (void) interrupt 19{
	VISU_INT7_START = 1;
	VISU_INT7_START = 0;
	VISU_INT7_WIDTH = 1;
	ETAT_LED = !ETAT_LED;
	P3IF &= (0<<7);	 
	VISU_INT7_END = 1;
	VISU_INT7_END = 0;
}

void main (void) {
	
				EA=0;
	      Init_Device();
				Port_INT_Init();
        while(1)
        {  
					if(ETAT_LED == clignotement)
					{
						 LED = LED_ON;
					   Software_Delay(2); // Allumaghe 20ms
					   LED = LED_OFF;
					   Software_Delay(10); // Extinction 100ms
					}
					else
					{
						LED = LED_OFF;
					}
        }						               	
			}



//*****************************************************************************	 
