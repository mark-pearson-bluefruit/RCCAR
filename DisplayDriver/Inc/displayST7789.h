#pragma once

enum {DISPLAY_WIDTH = 240,
	  DISPLAY_HEIGHT = 320};

enum { BUFFER_SIZE_BYTES_RGB565 = 2*DISPLAY_WIDTH*DISPLAY_HEIGHT
};


struct Display_HandleTypeDef {
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* portCS; uint16_t pinCS;
	GPIO_TypeDef* portDC; uint16_t pinDC;
	GPIO_TypeDef* portRST; uint16_t pinRST;
	GPIO_TypeDef* portBL; uint16_t pinBL;
} typedef Display_HandleTypeDef;


void setupDisplay(Display_HandleTypeDef _hDisplay);

void invertDisplay(void);
void paintDisplayRGB565(uint8_t* image);
void paintDisplayBW1(uint8_t* image);
void paintLineBW1(uint16_t lineID, uint8_t* lineData);
