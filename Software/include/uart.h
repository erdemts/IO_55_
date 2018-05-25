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
#define	STX				'<' //Start of text(command)
#define ETX				'>' //End of Text(command)
#define MAX_DATA_SIZE                   16

// UART Baud Rate Definitions
#define	BAUD_2400				( 1 )
#define	BAUD_9600				( 2 )
#define	BAUD_19200				( 3 )
#define	BAUD_57600				( 4 )

//// Uart Machine States
//#define UART_IDLE				( 0 )
//#define UART_WAIT_STX                           ( 1 )
//#define UART_WAIT_COMMAND                       ( 2 )
//#define UART_WAIT_DATA				( 3 )
//#define UART_WAIT_ETX    			( 4 )
//
//#define WAIT_d				( 6 )
//#define WAIT_DATA_SIZE				( 7 )
//#define WAIT_DATA				( 8 )
//#define WAIT_CRCH				( 9 )
//#define WAIT_CRCL				( 10 )
//#define RUN_COMMAND				( 11 )

//Uart timeout count
//#define UART_TIMEOUT_DELAY                      ( 20 )

#define UART_TRANSMIT_BUFFER_SIZE                     ( 60 )//Master Address, Slave Address, Command, DataSize, 8 Data, CRCH, CRCL. Total 14 Bytes per packet
#define UART_RECEIVE_BUFFER_SIZE                     ( 90 )//STX, Command, 8 Data, ETX. Total 10 Bytes per packet



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
 * Function     : void checkError( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void putch( unsigned char c );



#endif	// #ifndef _UART_H


