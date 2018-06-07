/*******************************************************************************
 *
 * FileName     : StringSerialProctocol.c
 * Dependencies :
 * Description  : String based serial communication protocol
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
 * 1.0        Erdem Tayfun Salman       05/06/2018       Initial Version
 ******************************************************************************/

/* INCLUDED FILES HERE -------------------------------------------------------*/
#include <htc.h>
#include "main.h"
#include "uart.h"
/* DEFINE LOCAL CONSTANTS HERE -----------------------------------------------*/
#define UART_RECEIVE_BUFFER_SIZE	60
#define UART_TRANSMIT_BUFFER_SIZE	60

#define STX							'<'
#define ETX 						'>'
#define DATA_SEPERATOR				':'

#define MAX_COMMAND_SIZE			5
#define MAX_DATA_SIZE				6

//COMMANDS
#define CMD_ROLE	1
#define CMD_PULSE   2
const char ROLE[] = "ROLE";
const char PULSE[] = "PULSE";

/* DEFINE LOCAL TYPES HERE ---------------------------------------------------*/
typedef enum {
    /* Application's state machine's initial state. */
    IDLE = 0,
    WAIT_COMMAND,
    WAIT_DATA,
    RUN_COMMAND

} APP_STATES;

/* DEFINE LOCAL MACROS HERE --------------------------------------------------*/
/* DEFINE LOCAL VARIABLES HERE -----------------------------------------------*/
volatile unsigned char uartReceiveBuffer[UART_RECEIVE_BUFFER_SIZE];
volatile unsigned char uartReceiveBufferR = 0;
volatile unsigned char uartReceiveBufferW = 0;

volatile unsigned char uartTransmitBuffer[UART_TRANSMIT_BUFFER_SIZE];
volatile unsigned char uartTransmitBufferR = 0;
volatile unsigned char uartTransmitBufferW = 0;

unsigned char dataBuffer[MAX_DATA_SIZE];
unsigned char dataCounter = 0;
unsigned char commandCharCounter = 0;
unsigned char commandBuffer[MAX_COMMAND_SIZE];

APP_STATES state = IDLE;

/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
extern volatile signed int temperature_mV;
extern volatile signed int currentTemperature;
extern char printBuffer[6];

/* DECLARE LOCAL FUNCTIONS HERE ----------------------------------------------*/
unsigned char cGetFromReceiveBuffer(void);
unsigned char CheckCommand(unsigned char command);

/* DEFINE FUNCTIONS HERE -----------------------------------------------------*/

/*******************************************************************************
 *
 * Function     : void vAddToUartReceiveBuffer(unsigned char data)
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vAddToUartReceiveBuffer(unsigned char data) {
    uartReceiveBuffer[ uartReceiveBufferW ] = data;
    uartReceiveBufferW++;
    if (uartReceiveBufferW == UART_RECEIVE_BUFFER_SIZE) {
        uartReceiveBufferW = 0;
    }
}

/*******************************************************************************
 *
 * Function     : void vAddToUartTransmitBuffer(unsigned char data)
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vAddToUartTransmitBuffer(unsigned char data) {
    uartTransmitBuffer[uartTransmitBufferW] = data;
    uartTransmitBufferW++;
    if (uartTransmitBufferW == UART_TRANSMIT_BUFFER_SIZE) {
        uartTransmitBufferW = 0;
    }
}

/*******************************************************************************
 *
 * Function     : unsigned char cGetFromTxBuffer(void)
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned char cGetFromTxBuffer(void) {
    unsigned char value;

    value = uartTransmitBuffer[uartTransmitBufferR];
    uartTransmitBufferR++;
    if (uartTransmitBufferR == UART_TRANSMIT_BUFFER_SIZE) {
        uartTransmitBufferR = 0;
    }
    return value;
}

void StringSerialProtocolService(void) {
    unsigned char i;
    unsigned char lastReceivedChar = 0;
    unsigned char pulseDelay = 0;

    if (uartReceiveBufferW != uartReceiveBufferR) {
        lastReceivedChar = cGetFromReceiveBuffer();

        if (lastReceivedChar == STX) {
            state = WAIT_COMMAND;
            commandCharCounter = 0;
            dataCounter = 0;
        } else if (state == WAIT_COMMAND) {
            if (lastReceivedChar == DATA_SEPERATOR) {
                state = WAIT_DATA;
            } else if (commandCharCounter < MAX_COMMAND_SIZE) {
                commandBuffer[commandCharCounter] = lastReceivedChar;
                ++commandCharCounter;
            } else {
                state = IDLE;
            }
        } else if (state == WAIT_DATA) {
            if (lastReceivedChar == ETX) {
                state = RUN_COMMAND;
            } else if (dataCounter < MAX_DATA_SIZE) {
                dataBuffer[dataCounter] = lastReceivedChar;
                ++dataCounter;
            } else {
                state = IDLE;
            }
        }

        if (state == RUN_COMMAND) {
            if (CheckCommand(CMD_ROLE)) {
                for (i = 0; i < DEVICE_RELAY_COUNT; ++i) {
                    if (dataBuffer[i] == 'A')
                        vSetRelay(i + 1, 1);
                    else if (dataBuffer[i] == 'K')
                        vSetRelay(i + 1, 0);
                    else if (dataBuffer[i] == '0') {
                        vPulseRelay(i + 1, 50);
                    } else if (((dataBuffer[i] - 48) > 0) && ((dataBuffer[i] - 48) <= 9)) {
                        vPulseRelay(i + 1, (dataBuffer[i] - 48)*5);
                    }
                }
                vAddToUartTransmitBuffer('<');
                vAddToUartTransmitBuffer('O');
                vAddToUartTransmitBuffer('K');
                vAddToUartTransmitBuffer('>');
                vSendDataToUart();
                setCommunicationTimeOut(COMMUNICATION_TIMEOUT_VALUE); //Cihaza module x sn boyunca bir komut gelmezse ethernet modul resetlenir.
            }
            else if (CheckCommand(CMD_PULSE)) {
                if (dataCounter == 5) {
                    pulseDelay = ((dataBuffer[1] - 48) * 1000) + ((dataBuffer[2] - 48) * 100) + ((dataBuffer[3] - 48) * 10) + (dataBuffer[4] - 48);
                    vPulseRelay((dataBuffer[0] - 48), pulseDelay);
                    vAddToUartTransmitBuffer('<');
                    vAddToUartTransmitBuffer('O');
                    vAddToUartTransmitBuffer('K');
                    vAddToUartTransmitBuffer('>');
                    vSendDataToUart();
                    setCommunicationTimeOut(COMMUNICATION_TIMEOUT_VALUE); //Cihaza module x sn boyunca bir komut gelmezse ethernet modul resetlenir.
                }
                else
                {
                    vAddToUartTransmitBuffer('<');
                    vAddToUartTransmitBuffer('H');
                    vAddToUartTransmitBuffer('A');
                    vAddToUartTransmitBuffer('T');
                    vAddToUartTransmitBuffer('A');
                    vAddToUartTransmitBuffer('>');
                    vSendDataToUart();
                    setCommunicationTimeOut(COMMUNICATION_TIMEOUT_VALUE); //Cihaza module x sn boyunca bir komut gelmezse ethernet modul resetlenir.
                }

            }
            state = IDLE;
        }
    }
}




/* DEFINE LOCAL FUNCTIONS HERE -----------------------------------------------*/

/*******************************************************************************
 *
 * Function     : unsigned char cGetFromReceiveBuffer(void)
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned char cGetFromReceiveBuffer(void) {
    unsigned char value;

    value = uartReceiveBuffer[uartReceiveBufferR];
    uartReceiveBufferR++;
    if (uartReceiveBufferR == UART_RECEIVE_BUFFER_SIZE) {
        uartReceiveBufferR = 0;
    }
    return value;
}

unsigned char CheckCommand(unsigned char command) {
    unsigned char result = 0;
    unsigned char i;
    unsigned char size = 0;

    result = 1;
    switch (command) {
        case CMD_ROLE:
            size = sizeof (ROLE) - 1;

            for (i = 0; i < size; ++i) {
                if (commandBuffer[i] != ROLE[i]) {
                    result = 0;
                    break;
                }
            }
            break;
        case CMD_PULSE:
            size = sizeof (PULSE) - 1;

            for (i = 0; i < size; ++i) {
                if (commandBuffer[i] != PULSE[i]) {
                    result = 0;
                    break;
                }
            }
            break;
        default:
            result = 0;
            break;
    }
    return result;
}












