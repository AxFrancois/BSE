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
void main (void) {
	
	      Init_Device();
	
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
				 if(BP == BP_ON && ACK_BP == BP_OFF)
				 {
					 ACK_BP = BP_ON;
					 if(ETAT_LED == clignotement)
					 {
						 ETAT_LED = eteinte;
					 }else
					 {
						 ETAT_LED = clignotement;
					 }
				 }
				 else if(BP == BP_OFF)
				 {
					 ACK_BP = BP_OFF;
				 }
        }						               	
			}
//*****************************************************************************	 
