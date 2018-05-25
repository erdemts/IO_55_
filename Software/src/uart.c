/*******************************************************************************
 *
 * FileName     : uart.c
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
#include "includes.h"

/* DEFINE LOCAL CONSTANTS HERE -----------------------------------------------*/
/* DEFINE LOCAL TYPES HERE ---------------------------------------------------*/
/* DEFINE LOCAL MACROS HERE --------------------------------------------------*/
/* DEFINE LOCAL VARIABLES HERE -----------------------------------------------*/
// Uart Variables
/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
/* DECLARE LOCAL FUNCTIONS HERE ----------------------------------------------*/
/* DEFINE FUNCTIONS HERE -----------------------------------------------------*/
/*******************************************************************************
 *
 * Function     : void vInitializeUart( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vInitializeUart( void )
{
    unsigned char cBaudTemp;

    cBaudTemp = BAUD_9600;

    UARTTX_TRIS	= 1;
    UARTRX_TRIS	= 1;
   
    if( cBaudTemp == BAUD_2400 )
    {
        BRGH = 0;
        BRG16 = 0;
        SPBRG = 129;
    }
    else if( cBaudTemp == BAUD_9600 )
    {
        BRGH = 0;
        BRG16 = 1;
        SPBRG =  51; //51 for 8 MHZ//129 for 20MHz
    }
    else if( cBaudTemp == BAUD_19200 )
    {
        BRGH = 0;
        BRG16 = 1;
        SPBRG = 64;
    }
    else if( cBaudTemp == BAUD_57600 )
    {
        SPBRG	=	34;
        BRGH	=	1;
        BRG16	=	1;
    }
	
    SYNC	= 0;
    SPEN	= 1;
    CREN	= 1;
    TXEN	= 1;
    TX9		= 0;	// 9 bit transmission disable
    RX9		= 0;	// 9 bit reception disable
    ADDEN	= 0;	// address dedection disable
    RCIE	= 1;
    TXIE	= 0;
}

/*******************************************************************************
 *
 * Function     : void checkError( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vCheckError( void )
{
    unsigned char a = 0;
    if ( OERR )
    {
        CREN  =0;
        CREN = 1;
    }
    if( FERR )
    {
        a = RCREG;
    }
}

/*******************************************************************************
 *
 * Function     : void checkError( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void putch( unsigned char c )
{
    while( !TXIF )
        continue;
    TXREG = c;
}



/* DEFINE LOCAL FUNCTIONS HERE -----------------------------------------------*/

