#pragma once

struct dataPin {
	GPIO_TypeDef* port;
	uint16_t pin;
};

void CAMSetup(GPIO_TypeDef *_GPIO_VS, uint16_t _GPIO_Pin_VS,
		      GPIO_TypeDef *_GPIO_HS, uint16_t _GPIO_Pin_HS,
			  GPIO_TypeDef *_GPIO_RST, uint16_t _GPIO_Pin_RST,
			  GPIO_TypeDef *_GPIO_PWNN, uint16_t _GPIO_Pin_PWNN,
			  GPIO_TypeDef* _GPIO_PCLK, uint16_t _GPIO_Pin_PCLK, struct dataPin* _dataPins);

void getFrame(uint8_t* pixelData, int sizeOfBuffer);
