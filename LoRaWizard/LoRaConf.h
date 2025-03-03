/*
 * LoraConf.h
 *
 *  Created on: Feb 10, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#ifndef LORACONF_H_
#define LORACONF_H_

#include "LoRaEvents.h"
#include <stdint.h>
#include "radio_driver.h"
#include "subghz.h"

typedef struct {
	uint32_t rf_frequency;
	uint8_t tx_output_power;
	uint8_t lora_bandwidth;
	uint8_t lora_spreading_factor;
	uint8_t lora_codingrate;
	uint8_t lora_preamble_length;
	uint8_t lora_symbol_timeout;

} LoRaConfig;

extern PacketParams_t packetParams;
extern const RadioLoRaBandwidths_t Bandwidths[];

LoRaConfig getDefaultLoRaConfig(void);
LoRaConfig createLoRaConfig(LoRaConfig conf);
void LoraInit(LoRaConfig config);

#endif /* LORACONF_H_ */
