/*******************************************************************************
 *
 * FileName     : BusIdDongle main.c
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


#include <htc.h>
#include <stdlib.h>
#include <stdio.h>
#include "HardwareProfile.h"
#include "uart.h"
#include "StringSerialProtocol.h"
#include "main.h"

__CONFIG(FOSC_INTRC_NOCLKOUT & WDTE_ON & PWRTE_ON & MCLRE_ON & CP_ON & CPD_ON & BOREN_OFF & IESO_OFF & FCMEN_OFF & LVP_OFF & DEBUG_OFF & BOR4V_BOR21V);

//#define DEBUG


/* DEFINE LOCAL TYPES HERE ---------------------------------------------------*/
typedef enum {
    /* Application's state machine's initial state. */
    CHECK_IO = 0,
    WAIT_DEBOUNCE
}IO_CHECK_STATES;

/* DEFINE LOCAL MACROS HERE --------------------------------------------------*/
#define bit_set(var,bitno)          ((var) |= 1 << (bitno))
#define bit_clear(var,bitno)        ((var) &= ~(1 << (bitno)))
#define bit_test(data,bitno)        ((data>>bitno)&0x01)
#define bit_test_int(data,bitno) 	((data>>bitno)&0x0001)
#define hibyte(x)                   (unsigned char)	((x>>8) & 0xFF)
#define lobyte(x)                   (unsigned char)	(x & 0xFF)

/* DEFINE LOCAL VARIABLES HERE -----------------------------------------------*/
//Timer variables
volatile unsigned char tim10Hz, tim2Hz, tim1sec, tim1min;
volatile unsigned int RELAY_1_Timer;
bit RELAY_1_Timer_En;
volatile unsigned int RELAY_2_Timer;
bit RELAY_2_Timer_En;
volatile unsigned int RELAY_3_Timer;
bit RELAY_3_Timer_En;
volatile unsigned int RELAY_4_Timer;
bit RELAY_4_Timer_En;
volatile unsigned int RELAY_5_Timer;
bit RELAY_5_Timer_En;

volatile unsigned int communicationControlTimer;
bit communicationControlTimer_En;
unsigned char inputPort;

volatile unsigned char debounceTimer;
bit debounceTimerEnable;
bit debounceTimerTimeoutFlag;


/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
extern volatile unsigned char uartTransmitBufferR;
extern volatile unsigned char uartTransmitBufferW;
/* DECLARE LOCAL FUNCTIONS HERE ----------------------------------------------*/
void vInitializeBoard(void);
void vSetRelay(unsigned char relay, unsigned char value);
void setDebounceTimer(unsigned char value);
void restartUSR_K2(void);
void InitializeInputs(void);
void CheckInputs(void);

/* DEFINE FUNCTIONS HERE -----------------------------------------------------*/
/* DEFINE LOCAL FUNCTIONS HERE -----------------------------------------------*/

/*******************************************************************************
 *
 * Function     :
 * PreCondition : None<
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
main(void) {
    vInitializeBoard();
    vInitializeUart();
    PEIE = 1;
    GIE = 1;
    ei();

    InitializeInputs();
    setCommunicationTimeOut(COMMUNICATION_TIMEOUT_VALUE); //Cihaza module 60 sn boyunca bir komut gelmezse ethernet modul resetlenir.

    while (1) {
        CLRWDT();
        CheckInputs();
        StringSerialProtocolService();
    }
}

void interrupt erdem(void) {
    unsigned char temp;
    //    unsigned int i;

    if (T0IE && T0IF) {
        TMR0 = 190; //  (1/2MHz)*256*66 = 8,4 ms
        
        if ((--debounceTimer == 0) && debounceTimerEnable) {
            debounceTimerTimeoutFlag = 1;
            debounceTimerEnable = 0;
        }

        if (--tim10Hz == 0)// (1/2MHz)*256*66*12 = 100mS
        {
            tim10Hz = 12;

            if ((--RELAY_1_Timer == 0) && RELAY_1_Timer_En) {
                RELAY_1 = 0;
                RELAY_1_Timer_En = 0;
            }
            if ((--RELAY_2_Timer == 0) && RELAY_2_Timer_En) {
                RELAY_2 = 0;
                RELAY_2_Timer_En = 0;
            }
            if ((--RELAY_3_Timer == 0) && RELAY_3_Timer_En) {
                RELAY_3 = 0;
                RELAY_3_Timer_En = 0;
            }
            if ((--RELAY_4_Timer == 0) && RELAY_4_Timer_En) {
                RELAY_4 = 0;
                RELAY_4_Timer_En = 0;
            }
            if ((--RELAY_5_Timer == 0) && RELAY_5_Timer_En) {
                RELAY_5 = 0;
                RELAY_5_Timer_En = 0;
            }
            if ((--communicationControlTimer == 0) && communicationControlTimer_En) {
                restartUSR_K2();
                communicationControlTimer = COMMUNICATION_TIMEOUT_VALUE;
            }

            if (--tim2Hz == 0) //500 mS
            {
                tim2Hz = 5;
                if (--tim1sec == 0) // 1 second
                {
                    tim1sec = 2;
                    LED_BLUE ^= 1;

                }//--------end period 1 sec.-----------------------------
            }//--------end period 2 Hz.-----------------------------
        }//--------end period 10 Hz.-----------------------------

        T0IF = 0;
        return;
    }

    if (RCIE && RCIF) {
        if (OERR) {
            CREN = 0;
            CREN = 1;
        }
        if (FERR) {
            temp = RCREG;
        }
        while (!RCIF) {
            vCheckError();
        }

        temp = RCREG;
        vAddToUartReceiveBuffer(temp);
        return;
    }

    if (TXIE && TXIF) {
        if (uartTransmitBufferR != uartTransmitBufferW) {
            temp = cGetFromTxBuffer();
            TXREG = temp;
        } else {
            TXIE = 0;
            TXIF = 0;
        }
        return;
    }
    return;
}

/*******************************************************************************
 *
 * Function     : void vPulseRelay(unsigned char relay, unsigned char timerValue)
 * PreCondition : None
 * Input        : relay: Relay No
 *              : timerValue : Relay pulse time x 100ms
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vPulseRelay(unsigned char relay, unsigned int timerValue) {
    switch (relay) {
        case 1:
            RELAY_1 = 1;
            RELAY_1_Timer = timerValue;
            RELAY_1_Timer_En = 1;
            break;
        case 2:
            RELAY_2 = 1;
            RELAY_2_Timer = timerValue;
            RELAY_2_Timer_En = 1;
            break;
        case 3:
            RELAY_3 = 1;
            RELAY_3_Timer = timerValue;
            RELAY_3_Timer_En = 1;
            break;
        case 4:
            RELAY_4 = 1;
            RELAY_4_Timer = timerValue;
            RELAY_4_Timer_En = 1;
            break;
        case 5:
            RELAY_5 = 1;
            RELAY_5_Timer = timerValue;
            RELAY_5_Timer_En = 1;
            break;
        default:
            break;
    }
}

/*******************************************************************************
 *
 * Function     : void vPulseRelay(unsigned char relay, unsigned char timerValue)
 * PreCondition : None
 * Input        : relay: Relay No
 *              : timerValue : Relay pulse time x 100ms
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vSetRelay(unsigned char relay, unsigned char value) {
    switch (relay) {
        case 1:
            RELAY_1 = value;
            break;
        case 2:
            RELAY_2 = value;
            break;
        case 3:
            RELAY_3 = value;
            break;
        case 4:
            RELAY_4 = value;
            break;
        case 5:
            RELAY_5 = value;
            break;
        default:
            break;
    }
}

/*******************************************************************************
 *
 * Function     : void vInitializeBoard(void)
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vInitializeBoard(void) {
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    OSCCON = 0b01110101;

    CM1CON0 = 0;
    CM2CON0 = 0;
    C1ON = 0;
    C2ON = 0;

    WDTCON = 0b00010110;
    OPTION_REG = 0b10000111; //pull ups disable TMR0 prescaler 1:256
    INTCON = 0;
    PIE1 = 0;
    PIE2 = 0;

    ANSELH = 0;
    ANSEL = 0;
    WPUB = 0;

    T0IF = 0;
    T0IE = 1;

    TRISA = 0b00000000; //
    TRISB = 0b00110111; //
    TRISC = 0b11000000; //
    TRISD = 0b00000000; //
    TRISE = 0b00000000; //
    ETH_RESET_TRIS = 0;
    ETH_RESET = 0;

    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    tim10Hz = 5;
    tim2Hz = 5;
    tim1sec = 2;
}

void setCommunicationTimeOut(unsigned int value) {
    communicationControlTimer = value;
    communicationControlTimer_En = 1;
}

//100us * value
void setDebounceTimer(unsigned char value) {
    debounceTimer = value;
    debounceTimerEnable = 1;
    debounceTimerTimeoutFlag = 0;
}

void restartUSR_K2(void) {
    ETH_RESET = 1;
#ifdef DEBUG
    RELAY_5 = 1;
#endif
    __delay_ms(40);
    __delay_ms(40);
    __delay_ms(40);
    __delay_ms(40);
    __delay_ms(40);
    __delay_ms(40);
    __delay_ms(40);
    ETH_RESET = 0;

#ifdef DEBUG
    RELAY_5 = 0;
#endif
    setCommunicationTimeOut(COMMUNICATION_TIMEOUT_VALUE); //Cihaza module x sn boyunca bir komut gelmezse ethernet modul resetlenir.
}

void InitializeInputs(void) {
    inputPort = (PORTB & 0b00110111);
}

void CheckInputs(void) {
    static IO_CHECK_STATES state;

    if (state == CHECK_IO) {
        if (inputPort != (PORTB & 0b00110111)) {
            setDebounceTimer(5); //200*100us = 20ms
            state = WAIT_DEBOUNCE;
        }
    } else if ((state == WAIT_DEBOUNCE) && debounceTimerTimeoutFlag) {
        debounceTimerTimeoutFlag = 0;
        if (inputPort != (PORTB & 0b00110111)) {
            inputPort = (PORTB & 0b00110111);

                vAddToUartTransmitBuffer('<');
                vAddToUartTransmitBuffer('D');
                vAddToUartTransmitBuffer('U');
                vAddToUartTransmitBuffer('R');
                vAddToUartTransmitBuffer('U');
                vAddToUartTransmitBuffer('M');
                vAddToUartTransmitBuffer(':');
                vAddToUartTransmitBuffer(INPUT_1 + 48);
                vAddToUartTransmitBuffer(INPUT_2 + 48);
                vAddToUartTransmitBuffer(INPUT_3 + 48);
                vAddToUartTransmitBuffer(INPUT_4 + 48);
                vAddToUartTransmitBuffer(INPUT_5 + 48);
                vAddToUartTransmitBuffer('0');
                vAddToUartTransmitBuffer('>');
                vSendDataToUart();
        }
        state = CHECK_IO;
    }
}

