/*
 * mainLoRa.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#ifndef MAINLORA_H_
#define MAINLORA_H_

#include "LoraConf.h"
#include "LoRaEvents.h"
#include "LoRaMessageHandler.h"

typedef struct {
    LoRaState state;
    LoRaSubState subState;
    uint32_t rxTimeout;
    uint32_t rxMargin;
    char rxBuffer[RX_BUFFER_SIZE];
    uint8_t rxSize;
} ConfigRES;

void transitionRxDone(ConfigRES *const RES);
void mainLoRa(void);

#endif /* MAINLORA_H_ */
