/*******************************************************************************
 *
 * FileName     : uart.h
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

#ifndef _UART_H
#define _UART_H

/* DEFINE CONSTANTS HERE -----------------------------------------------------*/
/* DEFINE TYPES HERE ---------------------------------------------------------*/
/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
/* DEFINITION OF MACROS ------------------------------------------------------*/
/* DECLARATION OF FUNCTIONS --------------------------------------------------*/
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
void vInitializeUart( void );

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
void vCheckError( void );

/*******************************************************************************
 *
 * Function     : void putch( unsigned char c )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void putch( unsigned char c );

/*******************************************************************************
 *
 * Function     : void vSendDataToUart(void)
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vSendDataToUart(void);


#endif	// #ifndef _UART_H


