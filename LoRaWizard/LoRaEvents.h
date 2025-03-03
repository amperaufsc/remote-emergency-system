/*
 * LoRaEvents.h
 *
 *  Created on: Feb 26, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#ifndef LORAEVENTS_H_
#define LORAEVENTS_H_

#include <stdint.h>
#include "radio_driver.h"
#include "subghz.h"

typedef enum {
	IRQ_TX_DONE_STATUS = 0x00U, // TX Success
	IRQ_RX_DONE_STATUS = 0X01U, // RX Success
	IRQ_RX_TX_TIMEOUT_STATUS = 0x02U, // TX and RX Timeout
	IRQ_CRC_ERROR_STATUS = 0x03U, // Error
	IRQ_DEFAULT_RETURN_STATUS = 0x99U // Default Return
} LoRaIrqStatus;

extern volatile LoRaIrqStatus lastEventStatus;

void RadioOnDioIrq(RadioIrqMasks_t radioIrq);
LoRaIrqStatus LoRaEventHandler(uint16_t irqStatus);
LoRaIrqStatus getLastLoRaEvent(void);

#endif /* LORAEVENTS_H_ */
