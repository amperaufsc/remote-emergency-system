/*
 * LoRaEvents.c
 *
 *  Created on: Feb 26, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#include "LoRaEvents.h"

volatile LoRaIrqStatus lastEventStatus = IRQ_DEFAULT_RETURN_STATUS;

void RadioOnDioIrq(RadioIrqMasks_t radioIrq) {
    LoRaEventHandler(radioIrq);
}

LoRaIrqStatus LoRaEventHandler(uint16_t irqStatus) {
    switch (irqStatus) {
        case IRQ_TX_DONE:
            return lastEventStatus = IRQ_TX_DONE_STATUS;
        case IRQ_RX_DONE:
            return lastEventStatus = IRQ_RX_DONE_STATUS;
        case IRQ_RX_TX_TIMEOUT:
            return lastEventStatus = IRQ_RX_TX_TIMEOUT_STATUS;
        case IRQ_CRC_ERROR:
            return lastEventStatus = IRQ_CRC_ERROR_STATUS;
        default:
            return lastEventStatus = IRQ_DEFAULT_RETURN_STATUS;
    }
}

LoRaIrqStatus getLastLoRaEvent(void) {
    return lastEventStatus;
}
