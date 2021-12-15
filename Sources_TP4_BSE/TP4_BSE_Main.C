//-----------------------------------------------------------------------------
// TP4_BSE_Main.c
// AUTH: Axel Francois, Elliot Garcia, Alexis Pincemin
// DATE: 5/12/21
//
// Target: C8051F02x
// Tool chain: KEIL Microvision5
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Cette application exécute les tâches suivantes:
// SYSCLK = Quartz externe = 22.1184 MHz (Visu sur P1.0)
// Clignotement de la LED P1.6
// Commande du clignotement de la LED avec le bouton poussoir P3.7 (INT7) et 
// via le signal DECL_EXT sur P3.6 (INT6)

// Mise en oeuvre de la base de temps Timer2 de 10ms et interruption Timer2

// Mise en oeuvre d'un comptage d'évènements SIG_IN (sur T4)
// Le comptage d'évènement est assuré par le Timer4 et par une interruption Timer4
// Comptage Programmable selon les facteurs 10-100-1000-10000
// La programmation du comptage est assurée au travers des niveaux sur P2.5 et P2.6
// Le signal SIG_OUT change d'état à chaque fois que l'on a compté 10...10000 évènements


// Calcul de la fréquence de SIG_IN et stockage dans la variable Frequence
// La mesure de fréquence est assurée par une mesure du nombre d'évènements sur une seconde
// Mesure réalisée dans l'interruption Timer2

// Visu Flag INT7 sur P2.4
// Visu Flag INT6 sur P6.4
// Visu Flag INT Timer2 sur P3.5
// Visu Flag INT Timer4 sur P3.4

// Visu SIG_OUT sur P3.3

//*****************************************************************************
// Fichiers d'entête
#include "intrins.h"
#include<c8051F020.h>
#include<c8051F020_SFR16.h>
#include<TP4_BSE_Lib_Config_Globale.h>
#include<TP4_BSE_Lib_Divers.h>
#include<TP4_BSE_Main.h>
#include <string.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
// Déclaration des MACROS

#define SYSCLK 22118400L
#define LED_ON 1

#define LED_OFF 0
#define LED_BLINK 0
#define BP_ON 0
#define BP_OFF 1
#define TO_BE_PROCESSED 1
#define PROCESSED 0
#define SET_VISU_INT6 P6 |= (1<<4)
#define RESET_VISU_INT6 P6 &= ~(1<<4)
//-----------------------------------------------------------------------------
// Déclarations Registres et Bits de l'espace SFR
sbit LED = P1^6;  // LED
sbit BP =P3^7;
sbit VISU_INT7 = P2^4;
sbit VISU_INT_TIMER2 = P3^5;
sbit VISU_INT_TIMER4 = P3^4;
sbit SIG_OUT = P3^3;
//-----------------------------------------------------------------------------
// Variables globales

bit Event = PROCESSED;  // Détection des évènements pour changer le clignotement de la LED
unsigned int Event_to_Count = 100 ; //Valeur du comptage d'évènements 
long frequence = 0;  // Fréquence calculée de SIG_IN
unsigned int CP_Overflow_Timer4; // Compteur d'overflows du Timer4
char c = 69;
char buffer[7];
int nbrEvent;
char keyPressed[3];
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {
	


 	   // Configurations globales
	      Init_Device();
	     
	   // Configurations  spécifiques  
	      Config_INT7(); // Configuration de INT7
	      Config_INT6(); // Configuration de INT6
	      Config_Timer2_TimeBase();
        Config_Timer4_Event_Counter();
				CFG_Clock_UART0();
				CFG_UART0();
	   // Fin des configurations
	      
	      EA = 1;  // Validation globale des interruptions
	
// Boucle infinie	
         while(1){
					 /* //Partie 4
					 gets(buffer, sizeof(buffer)-1);
					 plusUn(buffer);
					 printf("%s",buffer);*/
					 
					 // Partie 3
					 //printf("Mesure frequence %ld\n",frequence);
					 
					 // Partie 5 
					 printf("Mesure frequence %ld\n",frequence);
					 //gets(keyPressed, sizeof(keyPressed)-1);
					 
					 
					 if(!strcmp(keyPressed,"00\0"))
					 {
					 printf("Nombre d'evenements a compter (10-10000)?\n");
					 scanf("%d",&nbrEvent);
					 printf("Valeur retenue : %d\n",nbrEvent);
					 keyPressed[0] = '0';
					 keyPressed[1] = '0';
					 keyPressed[2] = '\0';
					 }
					 
					 Software_Delay(1000);
				 }					 			               	
			}
//******************************************************************************
void Config_INT7(void)
{
	P3IF &= ~(1<<7); // IE7 mis à 0 pending flag de INT7 effacé
	P3IF &= ~(1<<3); // IE7CF mis à 0 - sensibilité int7 front descendant	
	
	EIP2 &= ~(1<<5);  // PX7 mis à 0 - INT7 priorité basse
	EIE2 |= (1<<5);  // EX7 mis à 1 - INT7 autorisée
}
//******************************************************************************
void ISR_INT7 (void) interrupt 19
{
	VISU_INT7 = 1;
	P3IF &= ~(1<<7); // IE3 mis à 0 - remise à zéro du pending flag de INT7 effacé
	Event = TO_BE_PROCESSED;
	VISU_INT7 = 0;
}	

//*****************************************************************************	 
//******************************************************************************
void Config_INT6(void)
{
	P3IF &= ~(1<<7); // IE6 mis à 0 pending flag de INT6 effacé
	P3IF &= ~(1<<2); // IE6CF mis à 0 - sensibilité int6 front descendant	
	
	EIP2 &= ~(1<<4);  // PX6 mis à 0 - INT7 priorité basse
	EIE2 |= (1<<4);  // EX6 mis à 1 - INT7 autorisée
}

//******************************************************************************
void ISR_INT6 (void) interrupt 18
{
	SET_VISU_INT6;
	P3IF &= ~(1<<6); // IE6 mis à 0 - remise à zéro du pending flag de INT6 effacé
	P3IF ^= (1<<2);   // Action sur IE6CF - Commutation Front montant / Front Descendant
	Event = TO_BE_PROCESSED;
	RESET_VISU_INT6;
}	

//*****************************************************************************	 
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
void ISR_Timer2 (void) interrupt 5
{
	static char CP_Cligno;
	static char CP_Seconde = 0;
	static bit STATE_LED = LED_BLINK;
	char Read_Cfg_Event;
	static unsigned int CP_Timer4 = 0;
	static unsigned int OLD_CP_Timer4 = 0;

	VISU_INT_TIMER2 = 1;
	
	
	if (TF2 == 1)
	{
		TF2 = 0;
	
	// Gestion de la mesure de frequence - Synthèse
  // Le calcul de la fréquence est fait toutes les secondes		
	CP_Seconde++;
	if (CP_Seconde >= 100) 
	{
		CP_Seconde = 0;
		T4CON &= ~(1<<2);   // TR4 = 0 -- Timer4 stoppé
		CP_Timer4 = T4;     // Lecture Compteur Timer4
		T4CON |= (1<<2);   // TR4 = 1 -- Timer4 redémarré
		// Fréquence = nbre total d'évènements par seconde
		frequence = ((CP_Overflow_Timer4) * (long)Event_to_Count) +
		            (CP_Timer4 - OLD_CP_Timer4);
		            
		OLD_CP_Timer4 = CP_Timer4;
		CP_Overflow_Timer4 = 0;
		
	}	
	// Gestion des évènements INT6 et INT7
	// pour gérer les modes de clignotement de la LED
		if (Event == TO_BE_PROCESSED)
						 {
							 Event = PROCESSED;
							 STATE_LED =  !STATE_LED;	
						 }
	// Gestion du mode de clignotement de la LED					 
    CP_Cligno++;
	  if (CP_Cligno > 11) CP_Cligno = 0;
    if (STATE_LED == LED_BLINK)
						{
							if (CP_Cligno < 2) LED = LED_ON;
							else LED = LED_OFF;
						}
						else LED = LED_OFF;		
						
	// Etape5 - Lecture consigne du nbre d'évènements à compter	pour générer SIG_OUT			
    Read_Cfg_Event = (P2 & ((1<<5)|(1<<6)))>>5; // Lecture consigne sur P2.5 et P2.6
    switch (Read_Cfg_Event)
		{
    	case 0:
				Event_to_Count = 10; 
    		break;
    	case 1:
				Event_to_Count = 100; 
    		break;
			case 2:
				Event_to_Count = 1000; 
    		break;
			case 3:
				Event_to_Count = 10000; 
    		break;
    	default:
				Event_to_Count = 100; 
    		break;
    }
	}
	// Sécurité: si EXF2 est à 1 - RAZ de EXF2 	
	if (EXF2 == 1)
	{
		EXF2 = 0;
	}
	
	VISU_INT_TIMER2 = 0;
}	


//******************************************************************************
void Config_Timer4_Event_Counter(void) // Pour étape 4
{
	// Timer 4 configuré en compteur d'évènements SIG_IN
	// SIG_IN câblé sur l'entrée T4 du Timer4
	
	// Timer4 en mode autorechargement
	// Mode Counter
	// CLKTimer = SIG_IN
	// Valeur rechargement= (65536 - Nbre_Event_to_count)
	// L'overflow, et donc l'interruption Timer4 signale le comptage de 100 évènements
	T4CON = 0x02;   // Flags TF4 et EXF4 effacés
	                // RCLK1 et TCLK1 à zéro
	                // Transitions sur T4EX ignorées
	                // Timer stoppé
	                // Timer en mode counter
	                // Mode auto-reload
  RCAP4 = 65536-Event_to_Count;
  T4 = RCAP4;
  T4CON |= (1<<2);      // TR4 = 1 -- Timer2 démarré
  EIP2 |= (1<<2);      //PT4 = 0 -- Priorité Timer4 haute
  EIE2 |= (1<<2);      //ET4 = 1 --  INT Timer4 autorisée
}

//******************************************************************************
void ISR_Timer4 (void) interrupt 16  // Etape4
{
	
	
	VISU_INT_TIMER4 = 1; 
	
	if ((T4CON & (1<<7)) != 0)   // Test TF4 = 1
	{
		T4CON &= ~(1<<7);    // RAZ TF4
		SIG_OUT = !SIG_OUT;	  // Génération SIG_OUT
		// Prise en compte d'un comptage d'évènements configurable
		// La lecture de la config 10-100-1000-10000 est faite dans ISR_Timer2
    RCAP4 = 65536 - Event_to_Count;	//Reload compteur d'évènements
    // Comptage des overflows (pour compter au delà de 65536)		
		CP_Overflow_Timer4++;
	}
	
	// Sécurité si T4EX est à 1
	if ((T4CON & (1<<6)) != 0)   // Au cas où: Test EXF4 = 1
	{	T4CON &= ~(1<<6); } // RAZ EXF4			
	
	VISU_INT_TIMER4 = 0;
}	

void CFG_Clock_UART0(void)
{
	TR1 = 0;
	CKCON |= 0x10;
	TMOD |= 0x20;
	TMOD &= 0x2f;
	
	TF1 = 0;
	TH1 = 184;
	ET1 = 0;
	TR1 = 1;
	
}

void CFG_UART0(void)
{
	RCLK0 = 0;
	TCLK0 = 0;
	PCON |= 0x80;
	SCON0 = 0x70;
	PCON &= 0xBF;
	TI0 = 1;
	
}
char _getkey(void)
{
	char c;
	while(!RI0){
	}
	
	c = SBUF0;
	RI0 = 0;
	
	return(c);
	
}


char Putchar(char c)
{
	while(!TI0);
	TI0 = 0;
	SBUF0 = c;	
	return c;
}

void plusUn(char *buf)
{
	int i;
	for(i = 0; i<strlen(buf);i++) {
		buf[i] += 1;
	}
}
