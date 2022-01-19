/*---------------------------------------------------------------------------
;
;
;
;
; 	FILE NAME  	: TP6_BSE_Lib_Cde_Servo.H
; 	TARGET MCUs	: C8051F020, 
; 	DESCRIPTION	: 
;
; 	REVISION 1.0
;
;---------------------------------------------------------------------------*/

#ifndef __TP6_BSE_Lib_Cde_Servo__
#define __TP6_BSE_Lib_Cde_Servo__

void Set_Csg_Servo(char);
void Gestion_cde_Servo(void);
void Init_Timer0(void);
void ISR_Timer0 (void);

#endif
