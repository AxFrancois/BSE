//******************************************************************************
// 	FILE NAME  	: TP6_BSE_Lib_ADC-DAC-VREF.C
// 	TARGET MCUs	: C8051F020, 
// 	DESCRIPTION	: 
//
// 	REVISION 1.0
//  Ressources utilis�es:
//                         - ADC0 (Voie AIN0.7)
//                         - DAC0,
//                         - VREF
//                         - TIMER3
//
//******************************************************************************

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "TP6_Def_Globales.h"
#include "TP6_BSE_Lib_ADC-DAC-VREF.h"

#define VREF 2430

static unsigned int Seuil_Haut;
static unsigned int Seuil_Bas;
//******************************************************************************
//******************************************************************************
// Timer3_Init     Base de temps � 10KHz
//******************************************************************************
void  Timer3_Init_Fech_ADC0(void)
{
	TMR3RL = -(SYSCLK/10000);  // Pour g�n�rer Fech � 10Khz
	TMR3CN = 0x06;  // Flag Timer effac�
	                // Timer 3 d�marr�
	                // CLKTimer3 = Sysclk
	                // CLKTimer = source interne
}
//******************************************************************************
//******************************************************************************
// Voltage_Reference_Init
//******************************************************************************
void  Voltage_Reference_Init(void)
{
	 REF0CN    = 0x03; // ADC0 voltage reference from VREF0 pin
	                   // ADC1 voltage reference from VREF1 pin
	                   // Internal Temperature sensor off
	                   // Internal bias generator ON
	                   // Internal Referece Buffer On
}
//******************************************************************************
//******************************************************************************
// ADC0_Init_Decl_T3
//   - Conversion sur AIN0.7
//   - D�clenchement de la conversion � l'overflow du Timer3
//   - Gain de 1
//   - R�sultat de conversion justifi� � droite
//************************************************************************
void  ADC0_Init_Decl_T3(void)
{
		AMX0CF = 0;  // Toutes les voies sont en unipolaire
	  AMX0SL = 0x07; // S�lection voie AIN7
	  ADC0CF    = 0x50; // AD0SC = 10 --> CLKsar = 2,01MHz
	                    // Gain =1
	  // Config ADC0CN
	  AD0TM = 1; // Tracking durant 3 SAR Clk apr�s mise � 1 de AD0BUSY
	  AD0INT = 1;  // RAZ flag interruption INT0
	  AD0CM0 = 1;
	  AD0CM1 = 0; // Conversion ADC d�clench�e par overflow Timer3
	  AD0WINT = 0; // RAZ Flag ADC0 Windows compare
    AD0LJST = 0; // justification � droite du r�sultat de la conversion
	               // config termin�e
	   EIE2  |= (1<<1);  // EADC0 Enabled - INT ADC0 autoris�e
	   EIP2  &= ~(1<<1); // Priorit� INT ADC0 basse
	
	  Seuil_Haut = 4096;
    Seuil_Bas = 0;
	
	  AD0EN = 1;   // ADC0 d�marr� 
}
//******************************************************************************
//*****************************************************************************
// Set_Seuils_ADC(unsigned int SeuilH,unsigned int SeuilB )
// Les valeurs de seuil exprim�es en mV sont converties en valeur �quivalentes
// en sortie d'ADC
//******************************************************************************
void Set_Seuils_ADC(unsigned int SeuilH_mV,unsigned int SeuilB_mV )
{
	Seuil_Haut = (4096L * SeuilH_mV)/VREF; // Conversion Tension (mV) --> Code 
	
	Seuil_Bas = (4096L * SeuilB_mV)/VREF;
}	
//******************************************************************************
//******************************************************************************
// DAC0_Init - Initialisation du DAC0
//******************************************************************************
void  DAC0_Init(void)
{
	DAC0CN &= ~((1<<4)|(1<<3));  // DAC output updates on a write to DAC0H 
	DAC0CN &= ~((1<<2)|(1<<1)|(1<<0));  // Valeur 12 bits cal�e � droite 
	DAC0CN |= (1<<7);       // DAC0EN =1  -- DAC Enabled
}
//******************************************************************************
//******************************************************************************
// ISR_ADC0 --- INTERRUPTION ADC0
// D�clench�e � chaque fin de conversion ADC0
//******************************************************************************
void ISR_ADC0 (void) interrupt  15
{
	unsigned int value;
	
	VISU_INT_ADC0 = 1; 
	if (AD0INT == 1)   // Fin de conversion suite � overflow timer 3
	 { 
		 AD0INT = 0;
		 value = ADC0;
		 if (value > Seuil_Haut) value = Seuil_Haut;
     else if	(value < Seuil_Bas) value = Seuil_Bas;
		 
		 DAC0L = value%256;  // Renvoi de la donn�e acquise sur DAC0
		 DAC0H = value/256;
		 
   }		 
	 if (AD0WINT == 1)  // Au cas o�...
	 { 
		 AD0WINT = 0;
   }		
  VISU_INT_ADC0 = 0; 	 
}

//******************************************************************************