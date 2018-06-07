/* 
 * File:   StringSerialProctocol.h
 * Author: erdemts1
 *
 * Created on 05 Haziran 2018 Sal?, 22:05
 */

#ifndef STRINGSERIALPROTOCOL_H
#define	STRINGSERIALPROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif

void vAddToUartReceiveBuffer(unsigned char data);
void vAddToUartTransmitBuffer(unsigned char data);
unsigned char cGetFromTxBuffer(void);
void StringSerialProtocolService(void);


#ifdef	__cplusplus
}
#endif

#endif	/* STRINGSERIALPROCTOCOL_H */

