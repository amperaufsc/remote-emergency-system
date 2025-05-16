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
#include <string.h>
#include <stdio.h>

enum CarState {
	NULL_STATE,
	READY,
	GO,
	EMERGENCY,
};

typedef struct {
    LoRaState state;
    LoRaSubState subState;
    uint32_t rxTimeout;
    uint32_t rxMargin;
    char rxBuffer[RX_BUFFER_SIZE];
    uint8_t rxSize;
} ConfigRES;

void mainLoRa(void);
void MasterController (ConfigRES RES);
void SlaveController (ConfigRES RES);

#endif /* MAINLORA_H_ */
