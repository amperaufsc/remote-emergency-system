/*
 * LoRaEvents.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#include "mainLoRa.h"
#include <stdio.h>

#define STATE_MODE 1

extern UART_HandleTypeDef huart2;

void transitionRxDone(ConfigRES *const RES)
{
  PacketStatus_t packetStatus;
  char uartBuff[50];

  // Workaround 15.3 in DS.SX1261-2.W.APP (because following RX w/ timeout sequence)
  SUBGRF_WriteRegister(0x0920, 0x00);
  SUBGRF_WriteRegister(0x0944, (SUBGRF_ReadRegister(0x0944) | 0x02));

  SUBGRF_GetPayload((uint8_t *)RES->rxBuffer, &RES->rxSize, 0xFF);
  SUBGRF_GetPacketStatus(&packetStatus);

  sprintf(uartBuff, "RssiValue=%d dBm, SnrValue=%d Hz\r\n", packetStatus.Params.LoRa.RssiPkt, packetStatus.Params.LoRa.SnrPkt);
  HAL_UART_Transmit(&huart2, (uint8_t *)uartBuff, strlen(uartBuff), HAL_MAX_DELAY);
}

void mainLoRa(void) {
    ConfigRES RES;
    LoraInit(getDefaultLoRaConfig());
    RES.state = setStateMode(STATE_MODE);
    RES.rxTimeout = 3000;
    RES.rxMargin = 200;

    SUBGRF_SetDioIrqParams( IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT | IRQ_CRC_ERROR,
        IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT | IRQ_CRC_ERROR,
        IRQ_RADIO_NONE,
        IRQ_RADIO_NONE );
    SUBGRF_SetSwitch(RFO_LP, RFSWITCH_RX);
    SUBGRF_SetRx(RES.rxTimeout << 6);

    if (RES.state == STATE_MASTER){
    	uint8_t result = SendMessage("ping");
        switch (result) {
            case IRQ_TX_DONE_STATUS:
                HAL_UART_Transmit(&huart2, (uint8_t *)"TX done\r\n", 18, HAL_MAX_DELAY);
                break;
            case IRQ_RX_TX_TIMEOUT_STATUS:
                HAL_UART_Transmit(&huart2, (uint8_t *)"Event TX Timeout\r\n", 18, HAL_MAX_DELAY);
                break;
            case IRQ_CRC_ERROR_STATUS:
                HAL_UART_Transmit(&huart2, (uint8_t *)"Error\r\n", 18, HAL_MAX_DELAY);
                break;
            case IRQ_DEFAULT_RETURN_STATUS:
                HAL_UART_Transmit(&huart2, (uint8_t *)"Default Return\r\n", 18, HAL_MAX_DELAY);
                break;
            default:
                HAL_UART_Transmit(&huart2, (uint8_t *)"Default Master\r\n", 18, HAL_MAX_DELAY);
                break;
        }
    } else if (RES.state == STATE_SLAVE){
    	uint8_t result = receiveMessage(RES.rxBuffer, 255, RES.rxTimeout);
        switch (result) {
            case IRQ_RX_DONE_STATUS:
            	transitionRxDone(&RES);
                char msg[50];
                sprintf(msg, "TX done: %c\r\n", RES.rxBuffer);
                HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
                break;
            case IRQ_RX_TX_TIMEOUT_STATUS:
                HAL_UART_Transmit(&huart2, (uint8_t *)"Event RX Timeout\r\n", 18, HAL_MAX_DELAY);
                break;
            case IRQ_CRC_ERROR_STATUS:
                HAL_UART_Transmit(&huart2, (uint8_t *)"Error\r\n", 18, HAL_MAX_DELAY);
                break;
            case IRQ_DEFAULT_RETURN_STATUS:
                HAL_UART_Transmit(&huart2, (uint8_t *)"Default Return\r\n", 18, HAL_MAX_DELAY);
                break;
            default:
                HAL_UART_Transmit(&huart2, (uint8_t *)"Default Slave\r\n", 18, HAL_MAX_DELAY);
                break;
        }
    }
}

