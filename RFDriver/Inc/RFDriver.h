#pragma once

void TXSetup(void);
void TXSend(uint8_t* data); //Max size 32 bytes
void RXSetup(void);
uint8_t RXReceive(uint8_t* data); // Needs to be at least 32 bytes
uint32_t RXRecieveStream(uint8_t* data);

void encodeData(uint8_t *data, uint16_t pot1, uint16_t pot2);
void decodeData(uint8_t *data, uint16_t* pot1, uint16_t* pot2);

struct RF_HandleTypeDef {
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* portCE; uint16_t pinCE;
	GPIO_TypeDef* portCSN; uint16_t pinCSN;
} typedef RF_HandleTypeDef;

void RFSetup(RF_HandleTypeDef _hRF);
