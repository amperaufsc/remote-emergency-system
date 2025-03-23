/*
 * LoRaEvents.c
 *
 *  Created on: Feb 26, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#include "LoRaEvents.h"

volatile LoRaIrqStatus lastEventStatus = IRQ_DEFAULT_RETURN_STATUS;

void RadioOnDioIrq(RadioIrqMasks_t radioIrq) {
	lastEventStatus = LoRaEventHandler(radioIrq);
}

LoRaIrqStatus LoRaEventHandler(RadioIrqMasks_t radioIrq) {
    switch (radioIrq) {
        case IRQ_TX_DONE:
            return IRQ_TX_DONE_STATUS;
        case IRQ_RX_DONE:
            return IRQ_RX_DONE_STATUS;
        case IRQ_RX_TX_TIMEOUT:
            return IRQ_RX_TX_TIMEOUT_STATUS;
        case IRQ_CRC_ERROR:
            return IRQ_CRC_ERROR_STATUS;
        default:
            return IRQ_DEFAULT_RETURN_STATUS;
    }
}

LoRaIrqStatus getLastLoRaEvent(void) {
    return lastEventStatus;
}
