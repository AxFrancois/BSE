//-----------------------------------------------------------------------------
// TP6_BSE_Main.c
// AUTH: FJ
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
// Mise en oeuvre de l'UART0 à 115200 Bd, pas de parité, 8 bits, 1 stop bit 
 
// Visu Flag INT7 sur P2.4
// Visu Flag INT6 sur P6.4
// Visu Flag INT Timer2 sur P3.5

//*****************************************************************************
//-----------------------------------------------------------------------------
// Fichiers d'entête
#include "intrins.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<c8051F020.h>
#include<c8051F020_SFR16.h>
#include<TP6_BSE_Lib_Config_Globale.h>
#include<TP6_BSE_Lib_Divers.h>
#include<TP6_BSE_Lib_Timers_UART.h>
#include<TP6_BSE_Lib_UART.h>
#include<TP6_BSE_Lib_ADC-DAC-VREF.h>
#include<TP6_BSE_Lib_Cde_Servo.h>
#include<TP6_BSE_Main.h>

#include<TP6_Def_Globales.h>

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {
	
char xdata *ptr_buf;	
char *ptr_Mode;	
char *ptr_char;	
unsigned int xdata SH,SB;
int xdata Csg_Servo;	
unsigned long xdata Frequence, Frequence_int;
unsigned int xdata Frequence_fract;	      	
char xdata buf[50];
char i;
int value;
	


	
 	   // Configurations globales
	      Init_Device();
	     
	   // Configurations  spécifiques  
	      Config_Timer2_TimeBase();
	      CFG_Clock_UART0();
	      CFG_UART0();
	      Voltage_Reference_Init();
        Timer3_Init_Fech_ADC0();
        ADC0_Init_Decl_T3();
	      DAC0_Init();
		  	Config_Timer4_Frequency_Meter ();
				Init_Buf_RX_UART0();
	   // Fin des configurations
	      
	      Enable_Interrupt(); // Validation globale des interruptions
				
				Transmit_to_UART0("   System TP6 OK \n\r \n\r Entrer une commande A - S - F\n\r ");
				Set_Csg_Servo(0);	  // Positionnement du servo à la position 0 degré
				
// BOUCLE INFINIE
// Scrutation des commandes envoyées par le terminal
// Les commandes envoyées par le terminal sont les suivantes:
// A (ou a) -- réglages des seuils d'ecrêtage pour l'acquisition
// S (ou s) -- Réglage de la rotation du sservomoteur
// F (ou f) -- Mesure de la fréquence du signal SIG_IN
         while(1)
         { 
					 
           if ((ptr_buf = Test_Data_Received_UART0()) != NULL)
					 {
					   switch (ptr_buf[0])
					   { 
               case 'A':
               case 'a':
              					Transmit_to_UART0("Reglage seuils\n\r ");
							          while((Transmit_to_UART0("Entrer Seuil Haut en mV 0-2400 \n\r "))==0);
							          while ((ptr_buf = Test_Data_Received_UART0()) == 0);
							          SH = atoi(ptr_buf);
							 
							          while((Transmit_to_UART0("Entrer Seuil Bas en mV 0-2400 \n\r "))==0);
							          while ((ptr_buf = Test_Data_Received_UART0()) == 0);
							          SB = atoi(ptr_buf);
							          if ((SH<=2400)&& (SB<SH))
							          {
													sprintf(buf,"Seuils Haut: %u mV -- Seuils Bas: %u mV \n\r ",SH,SB);
                          while((Transmit_to_UART0(buf))==0);
                           Set_Seuils_ADC(SH,SB);		// Modification des réglages des écrêtages					
												}	
												else 
												{
													while((Transmit_to_UART0("Valeurs non valides \n\r "))==0);
												}	
							          break;
							 case 'S':
							 case 's':
								        while((Transmit_to_UART0("Reglage servomoteur\n\r "))==0);
								        while((Transmit_to_UART0("Consigne Rotation Servo\n\r "))==0);
							          while((Transmit_to_UART0("Entrer angle +/-90 \n\r "))==0);
							          while ((ptr_buf = Test_Data_Received_UART0()) == 0);
							          Csg_Servo = atoi(ptr_buf);
							          if ((Csg_Servo>=-90)&& (Csg_Servo<=90))
							          {
													sprintf(buf,"Consigne Angle servo: %d \n\r ",Csg_Servo);
                          while((Transmit_to_UART0(buf))==0);
                           Set_Csg_Servo(Csg_Servo);	// Lancement d'une rotation Servo						
													
												}	
												else 
												{
													while((Transmit_to_UART0("Valeurs non valides \n\r "))==0);
												}	
							          break;
              
							 case 'F':
               case 'f':
              					while((Transmit_to_UART0("Mesure frequence\n\r "))==0);
							          Frequence = Mesure_frequence(); // Récupération de la fréquence instantanée de SIG_IN
							          if (Frequence != 0)
												{
	                         Frequence_int =  Frequence/1000;
							             Frequence_fract =  Frequence%1000;
							             sprintf(buf,"Frequence: %6lu.%u Hz\n\r",Frequence_int,Frequence_fract);
							             while((Transmit_to_UART0(buf))==0);
												}
												else while((Transmit_to_UART0("Frequence trop basse\n\r "))==0);
							          break;
							 default: break;
						
             }							
					}		 							
				}     				               	
}



  

