/*
 * LoRaMessageHandler.c
 *
 *  Created on: Mar 3, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#include "LoRaMessageHandler.h"

LoRaIrqStatus SendMessage(const char *message) {
	if (message == NULL || strlen(message) == 0) {
		return IRQ_CRC_ERROR_STATUS;  // Invalid message
	}

	HAL_Delay(100);

	SUBGRF_SetDioIrqParams( IRQ_TX_DONE | IRQ_RX_TX_TIMEOUT,
	                          IRQ_TX_DONE | IRQ_RX_TX_TIMEOUT,
	                          IRQ_RADIO_NONE,
	                          IRQ_RADIO_NONE );
	SUBGRF_SetSwitch(RFO_LP, RFSWITCH_TX);
	// Workaround 5.1 in DS.SX1261-2.W.APP (before each packet transmission)
	SUBGRF_WriteRegister(0x0889, (SUBGRF_ReadRegister(0x0889) | 0x04));
	packetParams.Params.LoRa.PayloadLength = strlen(message);
	SUBGRF_SetPacketParams(&packetParams);
	SUBGRF_SendPayload((uint8_t *)message, strlen(message), 0);

	//while (getLastLoRaEvent() == IRQ_DEFAULT_RETURN_STATUS);
	return getLastLoRaEvent();
}

LoRaIrqStatus receiveMessage(char *buffer, uint8_t bufferSize, uint16_t Timeout) {
    if (buffer == NULL || bufferSize <= 0) {
        return IRQ_CRC_ERROR_STATUS;  // Invalid buffer
    }

    SUBGRF_SetDioIrqParams(IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT | IRQ_CRC_ERROR | IRQ_HEADER_ERROR,
                           IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT | IRQ_CRC_ERROR | IRQ_HEADER_ERROR,
                           IRQ_RADIO_NONE,
                           IRQ_RADIO_NONE);
    SUBGRF_SetSwitch(RFO_LP, RFSWITCH_RX);
    packetParams.Params.LoRa.PayloadLength = 0xFF;
    SUBGRF_SetPacketParams(&packetParams);
    SUBGRF_SetRx(Timeout << 6);

    LoRaIrqStatus event = getLastLoRaEvent();

    if (event == IRQ_RX_DONE_STATUS) {
    	SUBGRF_GetPayload((uint8_t *)buffer, (uint8_t)bufferSize, 0xFF);
    	buffer[bufferSize] = '\0';
        return IRQ_RX_DONE_STATUS;
    }

    return event;
}

