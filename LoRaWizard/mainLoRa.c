/*
 * LoRaEvents.h
 *
 *  Created on: Mar 17, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#include "mainLoRa.h"

#define STATE_MODE 0

extern UART_HandleTypeDef huart2;
enum CarState car = STATE_NULL;
enum CarState LastState = STATE_NULL;
bool connected = false;

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

void CAN_SEND (){

}

void ChangeState (enum CarState state) {
	char uartBuff[255];
	switch (state){
		case READY:
			break;
		case GO:
			sprintf(uartBuff, "GO\r\n");
			HAL_UART_Transmit(&huart2, (uint8_t *)uartBuff, strlen(uartBuff), HAL_MAX_DELAY);
			HAL_GPIO_WritePin(EBS_GPIO_Port, EBS_Pin, 1);
			HAL_GPIO_WritePin(ShutDown_GPIO_Port, ShutDown_Pin, 1);
			break;
		case EMERGENCY:
			sprintf(uartBuff, "Emergency\r\n");
			HAL_UART_Transmit(&huart2, (uint8_t *)uartBuff, strlen(uartBuff), HAL_MAX_DELAY);
			HAL_GPIO_WritePin(EBS_GPIO_Port, EBS_Pin, 0);
			HAL_GPIO_WritePin(ShutDown_GPIO_Port, ShutDown_Pin, 0);
			break;
	}
}

void SlaveController (ConfigRES RES) {
	while (true) {
		receiveMessage(RES.rxBuffer, 20 , RES.rxTimeout);
		if (strncmp(RES.rxBuffer, "READY", strlen("READY")) == 0);
		else if (strncmp(RES.rxBuffer, "GO", strlen("GO")) == 0) ChangeState(GO);
		else if (strncmp(RES.rxBuffer, "EMERGENCY", strlen("EMERGENCY")) == 0) ChangeState(EMERGENCY);
		else if (strncmp(RES.rxBuffer, "CONNECTED", strlen("CONNECTED")) == 0){
			connected = true; // conta tempo a cada 3 s
		}
		RES.rxBuffer[0] = '\0';
	}
}

void MasterController (ConfigRES RES) {
	uint8_t count = 0;
	while (LastState == car) {
		SendMessage("CONNECTED");
	}
	if (car == READY) {
		while (count <= 50){
			count++;
			SendMessage("READY");
		}
		HAL_Delay(2000);
	} else if (car == GO){
		while (count <= 20){
			count++;
			SendMessage("GO");
		}
	} else if (car == EMERGENCY){
		while (count <= 20){
			count++;
			SendMessage("EMERGENCY");
		}
	}
	LastState = car;
}
