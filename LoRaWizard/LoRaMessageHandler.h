/*
 * LoRaMessageHandler.h
 *
 *  Created on: Mar 3, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#ifndef LORAMESSAGEHANDLER_H_
#define LORAMESSAGEHANDLER_H_

#include "LoraConf.h"

LoRaIrqStatus SendMessage(const char *message);
LoRaIrqStatus receiveMessage(char *buffer, uint8_t bufferSize, uint16_t Timeout);

#endif /* LORAMESSAGEHANDLER_H_ */
