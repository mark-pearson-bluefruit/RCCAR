#pragma once

void TXSetup(void);
void TXSend(uint8_t* data); //Max size 32 bytes
void RXSetup(void);
uint8_t RXReceive(uint8_t* data); // Needs to be at least 32 bytes
uint32_t RXRecieveStream(uint8_t* data);

void encodeData(uint8_t *data, uint16_t pot1, uint16_t pot2);
void decodeData(uint8_t *data, uint16_t* pot1, uint16_t* pot2);
void RFSetup(SPI_HandleTypeDef *_hspi, GPIO_TypeDef *_GPIO_CE, uint16_t _GPIO_Pin_CE,
		GPIO_TypeDef *_GPIO_CSN, uint16_t _GPIO_Pin_CSN);
