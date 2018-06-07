/*******************************************************************************
 *
 * FileName     : HardwareProfile.h ( BusIdDongle Hardware profile file )
 * Dependencies :
 * Description  :
 * Processor    : PIC16F887
 * Compiler     : Hi-tech Picc 9.83
 * Linker       :
 * Company      : Inno Technology Incorporated
 *
 * Software License Agreement
 * Copyright (C) 2011 - 2012 Inno Technology Inc.  All rights
 * reserved.
 *
 * Version    Author                     Date             Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 1.0        Erdem Tayfun Salman       11/14/2014       Initial Version
 ******************************************************************************/
#ifndef HARDWAREPROFILE_H
#define HARDWAREPROFILE_H

#include <htc.h>

#define _XTAL_FREQ	8000000


/*****************************************************************************
 *	Uart
 *****************************************************************************/
#define UARTTX_TRIS                 (TRISCbits.TRISC6)
#define UARTRX_TRIS                 (TRISCbits.TRISC7)

#define ETH_RESET_TRIS              (TRISCbits.TRISC3)
#define ETH_RESET                   (PORTCbits.RC3)

#define ETH_RELOAD_TRIS             (TRISCbits.TRISC2)
#define ETH_RELOAD                  (PORTCbits.RC2)


/*****************************************************************************
 *	Relays
 *****************************************************************************/
#define RELAY_5_TRIS            (TRISCbits.TRISC5)//RELAY_1
#define RELAY_5                 (PORTCbits.RC5)

#define RELAY_4_TRIS            (TRISCbits.TRISC4)
#define RELAY_4                 (PORTCbits.RC4)

#define RELAY_3_TRIS            (TRISDbits.TRISD3)
#define RELAY_3                 (PORTDbits.RD3)

#define RELAY_2_TRIS            (TRISDbits.TRISD2)
#define RELAY_2                 (PORTDbits.RD2)

#define RELAY_1_TRIS            (TRISDbits.TRISD1)
#define RELAY_1                 (PORTDbits.RD1)



/*****************************************************************************
 *	INPUT PINS
 *****************************************************************************/

#define INPUT_1_TRIS            (TRISBbits.TRISB0)
#define INPUT_1                 (PORTBbits.RB0)

#define INPUT_2_TRIS            (TRISBbits.TRISB1)
#define INPUT_2                 (PORTBbits.RB1)

#define INPUT_3_TRIS            (TRISBbits.TRISB2)
#define INPUT_3                 (PORTBbits.RB2)

#define INPUT_4_TRIS            (TRISBbits.TRISB4)
#define INPUT_4                 (PORTBbits.RB4)

#define INPUT_5_TRIS            (TRISBbits.TRISB5)
#define INPUT_5                 (PORTBbits.RB5)

/*****************************************************************************
 *	I/O Pins
 *****************************************************************************/
#define LED_BLUE                 (PORTDbits.RD0) //RED LED
#define LED_BLUE_TRIS		     (TRISDbits.TRISD0) //RED LED


/*****************************************************************************
 *	TEMPERATURE SENSORS
 *****************************************************************************/
#define LM94022_TEMP_SENS            (PORTBbits.RB5)
#define LM94022_TEMP_SENS_TRIS     (TRISBbits.TRISB5)

//Analog channels
#define LM9402                      (13)




#endif
