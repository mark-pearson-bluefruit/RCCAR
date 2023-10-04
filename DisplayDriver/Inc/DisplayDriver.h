#pragma once

void setupDisplay(SPI_HandleTypeDef* _hspi, GPIO_TypeDef *_GPIO_DC, uint16_t _GPIO_Pin_DC,
				  GPIO_TypeDef *_GPIO_RESET, uint16_t _GPIO_Pin_RESET);

void toggleDisplayInversion(void);
uint8_t* readDisplayInfo(void);
void softwareReset();
void readPixelFormat(void);
void turnOn();
void makeColor();
