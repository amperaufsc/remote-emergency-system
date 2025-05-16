/*
 * LoRaEvents.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#include "mainLoRa.h"

#define STATE_MODE 1

extern UART_HandleTypeDef huart2;
enum CarState car = STATE_NULL;

void mainLoRa(void) {
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
    ConfigRES RES;
    LoraInit(getDefaultLoRaConfig());
    RES.rxTimeout = 3000;
    RES.rxMargin = 200;
    RES.state = setStateMode(STATE_MODE);

    SUBGRF_SetDioIrqParams( IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT | IRQ_CRC_ERROR,
        IRQ_RX_DONE | IRQ_RX_TX_TIMEOUT | IRQ_CRC_ERROR,
        IRQ_RADIO_NONE,
        IRQ_RADIO_NONE );
    SUBGRF_SetSwitch(RFO_LP, RFSWITCH_RX);
    SUBGRF_SetRx(RES.rxTimeout << 6);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    while(true){
    	if (RES.state == STATE_MASTER) MasterController(RES);
    	else if (RES.state == STATE_SLAVE) SlaveController(RES);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
		case NextState_Pin:
			car++;
			uint8_t count = 0;
			if (car == READY) {
				while (count <= 100){
					count++;
					SendMessage("READY");
				}
				HAL_Delay(2000);
			} else if (car == GO){
				while (count <= 100){
					count++;
					SendMessage("GO");
				}
			} else if (car == EMERGENCY){
				while (count <= 100){
					count++;
					SendMessage("EMERGENCY");
				}
			}
			break;
    }
}

void CAN_SEND (){

}

uint8_t teste = 0;
void SlaveController (ConfigRES RES) {
	while (true) {
		receiveMessage(RES.rxBuffer, 20 , RES.rxTimeout);
		char uartBuff[255];
		sprintf(uartBuff, "%s\r\n", &RES.rxBuffer);
		HAL_UART_Transmit(&huart2, (uint8_t *)uartBuff, strlen(uartBuff), HAL_MAX_DELAY);
		if (strncmp(RES.rxBuffer, "READY", strlen("READY")) == 0){
			teste = 1;
		} else if (strncmp(RES.rxBuffer, "GO", strlen("GO")) == 0){
			teste = 2;
		} else if (strncmp(RES.rxBuffer, "EMERGENCY", strlen("EMERGENCY")) == 0){
			teste = 3;
		} else if (strncmp(RES.rxBuffer, "CONNECTED", strlen("CONNECTED")) == 0){
			teste = 4;
		}

		RES.rxBuffer[0] = '\0';
	}
}
