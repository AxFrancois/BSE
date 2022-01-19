//******************************************************************************
// 	FILE NAME  	: TP6_BSE_Lib_Cde_Servo.C
// 	TARGET MCUs	: C8051F020, 
// 	DESCRIPTION	: 
//
// 	REVISION 1.0
//  Ressources utilisées:
//                         - Timer0
//                         - 1 GPIO
//
//******************************************************************************/******************************************************************************

#include "C8051F020.h"
#include "c8051F020_SFR16.h"
#include "TP6_Def_Globales.h"
#include "TP6_BSE_Lib_Cde_Servo.h"
#include<stdio.h>


//****************************************************************************
// Specifications Servo
#define ROT_0 1400      // Durée impulsion en US pour une rotation de 0°
#define ROT_90 2200     // Durée impulsion en US pour une rotation de 90°
#define VT_ROT 300      // vitesse de rotation du servo en degré/seconde
                        //Vitesse minimisée d'un facteur 2
//****************************************************************************                         
static char xdata Position_actuelle = 90;
static char xdata Position_future;
static bit New_Cde_rot_servo = NO;
static unsigned int Preload_Value_T0=0;
static unsigned int Temps_rotation_ms;
int angle;
float angle_restant;
float fraction;
char xdata buf[50];
//*****************************************************************************
//*****************************************************************************
//  Préparation à une commande de rotation servo
//  selon l'angle transmis et la position actuelle
// Fonction appelée dans le main pour déclencher une rotation servo
//******************************************************************************
  void Set_Csg_Servo(char Csg_Servo)
 {
	Position_future = Csg_Servo;
	angle = Position_future-Position_actuelle;
	angle_restant = (float)abs(angle);
	Temps_rotation_ms = Position_future*(2200L-1400)/(90) + 1400;
	fraction = Temps_rotation_ms/(12*0.045);
	fraction = (int)fraction;
	Preload_Value_T0 = 0xFFFF+1-fraction;
	Position_actuelle = Position_future;
	Init_Timer0();
//	TH0 = Preload_Value_T0>>8;
//	TL0 = Preload_Value_T0 & 0x00FF;
	 
 }
//******************************************************************************
//*****************************************************************************
//  Gestion_cde_Servo()
//  Appelé dans la routine d'interruption de la base de temps de récurrence 10ms 
//  Gere l'utilisation du servomoteur
//******************************************************************************
void Gestion_cde_Servo(void)
{
	if(angle_restant>0){
			angle_restant -= 0.3*Temps_rotation_ms/100;
			Init_Timer0();
			TR0 = 1;
			PIN_Cde_Servo = 1;
	}
}

void Init_Timer0(void) {
	TR0 = 0;
	TMOD |= (1<<0);
	TMOD &= ~(1<<1);
	TMOD &= ~(1<<4);
	TMOD &= ~(1<<2);
	CKCON &= ~(1<<3);
	ET0 = 1;
	PT0 = 1;
	
	TH0 = Preload_Value_T0>>8;
	TL0 = Preload_Value_T0 & 0x00FF;
	
}

void ISR_Timer0 (void) interrupt 1 {
	TF0 = 0;
	PIN_Cde_Servo = 0;
	TR0 = 0;
}