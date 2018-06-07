/* 
 * File:   main.h
 * Author: erdemts1
 *
 * Created on 05 Haziran 2018 Sal?, 23:32
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#ifndef _XTAL_FREQ 
#define _XTAL_FREQ 8000000   
#endif

#define COMMUNICATION_TIMEOUT_VALUE         3000 //3000 x 100 ms = 5 minutes
#define DEVICE_RELAY_COUNT                  5   

    
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
void vSetRelay(unsigned char relay, unsigned char value);

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
void vPulseRelay(unsigned char relay, unsigned int timerValue) ;
void setCommunicationTimeOut(unsigned int value);



#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

