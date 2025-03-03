/*
 * LoraConf.c
 *
 *  Created on: Feb 10, 2025
 *      Author: Luiz Fernando Silvério Felisbino
 */

#include "LoRaConf.h"

PacketParams_t packetParams;
const RadioLoRaBandwidths_t Bandwidths[] = { LORA_BW_125, LORA_BW_250, LORA_BW_500 };

LoRaConfig getDefaultLoRaConfig (void) {
	return (LoRaConfig){
		.rf_frequency = 868000000,
		.tx_output_power = 14,
		.lora_bandwidth = 0,
		.lora_spreading_factor =7,
		.lora_codingrate = 1,
		.lora_preamble_length = 8,
		.lora_symbol_timeout = 5,
	};
}

LoRaConfig createLoRaConfig(LoRaConfig conf) {
    LoRaConfig config = getDefaultLoRaConfig();

    if (conf.rf_frequency > 0) config.rf_frequency = conf.rf_frequency;
    if (conf.tx_output_power > 0) config.tx_output_power = conf.tx_output_power;
    if (conf.lora_bandwidth >= 0) config.lora_bandwidth = conf.lora_bandwidth;
    if (conf.lora_spreading_factor >= 0) config.lora_spreading_factor = conf.lora_spreading_factor;
    if (conf.lora_codingrate >= 0) config.lora_codingrate = conf.lora_codingrate;
    if (conf.lora_preamble_length >= 0) config.lora_preamble_length = conf.lora_preamble_length;
    if (conf.lora_symbol_timeout >= 0) config.lora_symbol_timeout = conf.lora_symbol_timeout;

    return config;
}


void LoraInit(LoRaConfig config) {
	// Initialize the hardware (SPI bus, TCXO control, RF switch)
	SUBGRF_Init(RadioOnDioIrq);

	// Use DCDC converter if `DCDC_ENABLE` is defined in radio_conf.h
	// "By default, the SMPS clock detection is disabled and must be enabled before enabling the SMPS." (6.1 in RM0453)
	SUBGRF_WriteRegister(SUBGHZ_SMPSC0R, (SUBGRF_ReadRegister(SUBGHZ_SMPSC0R) | SMPS_CLK_DET_ENABLE));
	SUBGRF_SetRegulatorMode();

	// Use the whole 256-byte buffer for both TX and RX
	SUBGRF_SetBufferBaseAddress(0x00, 0x00);

	//Set Operating Frequency and Transmission Power
	SUBGRF_SetRfFrequency(config.rf_frequency);
	SUBGRF_SetRfTxPower(config.tx_output_power);

	//Preamble Interrupt Timer Configuration
	SUBGRF_SetStopRxTimerOnPreambleDetect(false);

	//Set Packet Type to LoRa
	SUBGRF_SetPacketType(PACKET_TYPE_LORA);

	//Sync Word Setup
	SUBGRF_WriteRegister( REG_LR_SYNCWORD, ( LORA_MAC_PRIVATE_SYNCWORD >> 8 ) & 0xFF );
	SUBGRF_WriteRegister( REG_LR_SYNCWORD + 1, LORA_MAC_PRIVATE_SYNCWORD & 0xFF );

	/*
	 * Set LoRa Modulation Parameters
	 * Bandwidth: Defined by config.lora_bandwidth, the bandwidth value is obtained from a Bandwidths array
	 * Coding rate: Defined by config.lora_codingrate
	 * Spreading Factor: Defined by config.lora_spreading_factor
	 * Low Data Rate Optimization: Disabled with value 0x00
	 */
	ModulationParams_t modulationParams;
	modulationParams.PacketType = PACKET_TYPE_LORA;
	modulationParams.Params.LoRa.Bandwidth = Bandwidths[config.lora_bandwidth];
	modulationParams.Params.LoRa.CodingRate = (RadioLoRaCodingRates_t)config.lora_codingrate;
	modulationParams.Params.LoRa.LowDatarateOptimize = 0x00;
	modulationParams.Params.LoRa.SpreadingFactor = (RadioLoRaSpreadingFactors_t)config.lora_spreading_factor;
	SUBGRF_SetModulationParams(&modulationParams);

	/*
	 * Set LoRa Packet Parameters
	 * CRC Mode: Enables cyclic redundancy checking (CRC) to ensure packet integrity
	 * HeaderType: Defines the packet header type as variable length
	 * InvertIQ: Sets the inversion of IQ polarity (normal)
	 * Preamble Length: Used the value from config.lora_preamble_length
	 */
	packetParams.PacketType = PACKET_TYPE_LORA;
	packetParams.Params.LoRa.CrcMode = LORA_CRC_ON;
	packetParams.Params.LoRa.HeaderType = LORA_PACKET_VARIABLE_LENGTH;
	packetParams.Params.LoRa.InvertIQ = LORA_IQ_NORMAL;
	packetParams.Params.LoRa.PayloadLength = 0xFF;
	packetParams.Params.LoRa.PreambleLength = config.lora_preamble_length;
	SUBGRF_SetPacketParams(&packetParams);

	// WORKAROUND - Optimizing the Inverted IQ Operation, see DS_SX1261-2_V1.2 datasheet chapter 15.4
	// RegIqPolaritySetup @address 0x0736
	SUBGRF_WriteRegister( 0x0736, SUBGRF_ReadRegister( 0x0736 ) | ( 1 << 2 ) );
}

