#include <displayNamesST7789.h>
#include "stm32g4xx_hal.h"
#include "displayST7789.h"


static Display_HandleTypeDef hDisplay;

// GPIO Functions
static void sendDisplayCommand(uint8_t command);
static void sendDisplayData(uint8_t* pData, uint16_t sizeOfData);


void sendDisplayOneByte(uint8_t data) {
	sendDisplayData(&data, 1);
}

void LCD_2IN_Init(void)
{
	//LCD_2IN_Reset();

	sendDisplayCommand(0x36);
	sendDisplayOneByte(0x00);

	sendDisplayCommand(0x3A);
	sendDisplayOneByte(0x05);

	sendDisplayCommand(0x21);

	sendDisplayCommand(0x2A);
	sendDisplayOneByte(0x00);
	sendDisplayOneByte(0x00);
	sendDisplayOneByte(0x01);
	sendDisplayOneByte(0x3F);

	sendDisplayCommand(0x2B);
	sendDisplayOneByte(0x00);
	sendDisplayOneByte(0x00);
	sendDisplayOneByte(0x00);
	sendDisplayOneByte(0xEF);

	sendDisplayCommand(0xB2);
	sendDisplayOneByte(0x0C);
	sendDisplayOneByte(0x0C);
	sendDisplayOneByte(0x00);
	sendDisplayOneByte(0x33);
	sendDisplayOneByte(0x33);

	sendDisplayCommand(0xB7);
	sendDisplayOneByte(0x35);

	sendDisplayCommand(0xBB);
	sendDisplayOneByte(0x1F);

	sendDisplayCommand(0xC0);
	sendDisplayOneByte(0x2C);

	sendDisplayCommand(0xC2);
	sendDisplayOneByte(0x01);

	sendDisplayCommand(0xC3);
	sendDisplayOneByte(0x12);

	sendDisplayCommand(0xC4);
	sendDisplayOneByte(0x20);

	sendDisplayCommand(0xC6);
	sendDisplayOneByte(0x0F);

	sendDisplayCommand(0xD0);
	sendDisplayOneByte(0xA4);
	sendDisplayOneByte(0xA1);

	sendDisplayCommand(0xE0);
	sendDisplayOneByte(0xD0);
	sendDisplayOneByte(0x08);
	sendDisplayOneByte(0x11);
	sendDisplayOneByte(0x08);
	sendDisplayOneByte(0x0C);
	sendDisplayOneByte(0x15);
	sendDisplayOneByte(0x39);
	sendDisplayOneByte(0x33);
	sendDisplayOneByte(0x50);
	sendDisplayOneByte(0x36);
	sendDisplayOneByte(0x13);
	sendDisplayOneByte(0x14);
	sendDisplayOneByte(0x29);
	sendDisplayOneByte(0x2D);

	sendDisplayCommand(0xE1);
	sendDisplayOneByte(0xD0);
	sendDisplayOneByte(0x08);
	sendDisplayOneByte(0x10);
	sendDisplayOneByte(0x08);
	sendDisplayOneByte(0x06);
	sendDisplayOneByte(0x06);
	sendDisplayOneByte(0x39);
	sendDisplayOneByte(0x44);
	sendDisplayOneByte(0x51);
	sendDisplayOneByte(0x0B);
	sendDisplayOneByte(0x16);
	sendDisplayOneByte(0x14);
	sendDisplayOneByte(0x2F);
	sendDisplayOneByte(0x31);
	sendDisplayCommand(0x21);

	sendDisplayCommand(0x11);

	sendDisplayCommand(0x29);
}

void resetDisplay(void) {
	HAL_GPIO_WritePin(hDisplay.portRST, hDisplay.pinRST, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(hDisplay.portRST, hDisplay.pinRST, GPIO_PIN_RESET);
	HAL_Delay(300);
	HAL_GPIO_WritePin(hDisplay.portRST, hDisplay.pinRST, GPIO_PIN_SET);
}

static void sendDisplayCommand(uint8_t command) {
	HAL_GPIO_WritePin(hDisplay.portDC, hDisplay.pinDC, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(hDisplay.portCS, hDisplay.pinCS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hDisplay.hspi, &command, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(hDisplay.portCS, hDisplay.pinCS, GPIO_PIN_SET);
}

static void sendDisplayData(uint8_t* pData, uint16_t sizeOfData) {
	HAL_GPIO_WritePin(hDisplay.portDC, hDisplay.pinDC, GPIO_PIN_SET);
	HAL_GPIO_WritePin(hDisplay.portCS, hDisplay.pinCS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hDisplay.hspi, pData, sizeOfData, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(hDisplay.portCS, hDisplay.pinCS, GPIO_PIN_SET);
}

static void sendDisplayDataWord(uint16_t word) {
	uint8_t wordSplit[2];
	wordSplit[0] = (word >> 8); wordSplit[1] = (word & 0xFF);
	sendDisplayData(wordSplit, 2);
}

void setupDisplay(Display_HandleTypeDef _hDisplay) {
	hDisplay = _hDisplay;
	resetDisplay();
	sendDisplayCommand(SWRESET);
	HAL_Delay(200);
	sendDisplayCommand(NORON);
	sendDisplayCommand(DISPON);
	HAL_GPIO_WritePin(hDisplay.portBL, hDisplay.pinBL, GPIO_PIN_SET);
	LCD_2IN_Init();
}

void setCursor(uint16_t x, uint16_t y) {
	uint8_t xPos[2];
	xPos[0] = (x>>8); xPos[1] = x&0xFF;
	uint8_t yPos[2];
	yPos[0] = (y>>8); yPos[1] = y&0xFF;
	sendDisplayCommand(CASET);
	sendDisplayData(xPos, 2);
	sendDisplayData(xPos, 2);

	sendDisplayCommand(RASET);
	sendDisplayData(yPos, 2);
	sendDisplayData(yPos, 2);
}

void setWindow(uint16_t xStart, uint16_t xEnd, uint16_t yStart, uint16_t yEnd) {
	sendDisplayCommand(CASET);
	sendDisplayDataWord(xStart);
	sendDisplayDataWord(xEnd);
	sendDisplayCommand(RASET);
	sendDisplayDataWord(yStart);
	sendDisplayDataWord(yEnd);
}

void paintPixel(uint16_t x, uint16_t y, uint16_t colour) {
	uint8_t pixelColor[2];
	pixelColor[0] = (colour>>8);
	pixelColor[1] = colour & 0xFF;
	setCursor(x,y);
	sendDisplayCommand(RAMWR);
	sendDisplayData(pixelColor, 2);
}

void paintDisplayRGB565(uint8_t* image) {
	const uint16_t bytesPerPixelRGB565 = 2;
	setWindow(0, DISPLAY_WIDTH, 0, DISPLAY_HEIGHT);
	sendDisplayCommand(RAMWR);
	for (uint16_t y = 0; y < DISPLAY_HEIGHT; y++) {
		sendDisplayData(image + bytesPerPixelRGB565*y*DISPLAY_WIDTH, bytesPerPixelRGB565*DISPLAY_WIDTH);
	}
}

void paintDisplayBW1(uint8_t* image) {
	const uint16_t bytesPerPixelRGB565 = 2;
	setWindow(0, DISPLAY_WIDTH, 0, DISPLAY_HEIGHT);
	sendDisplayCommand(RAMWR);
	uint8_t rowData[bytesPerPixelRGB565*DISPLAY_WIDTH];
	for (uint16_t y = 0; y < DISPLAY_HEIGHT; y++) {
		// Fill data BLACK 0x0000, WHITE 0xFFFF
		for (uint16_t x = 0; x < DISPLAY_WIDTH; x++) {
			rowData[2*x] = 0xFF * ((image[y*DISPLAY_WIDTH/8 + x/8] >> (7 - x%8)) & 0x01);
			rowData[2*x + 1] = 0xFF * ((image[y*DISPLAY_WIDTH/8 + x/8] >> (7 - x%8)) & 0x01);
		}

		sendDisplayData(rowData, bytesPerPixelRGB565*DISPLAY_WIDTH);
	}
}

void invertDisplay(void) {
	static int toggle = 0;
	if (toggle == 0) {
		sendDisplayCommand(INVON);
		toggle = 1;
	} else {
		sendDisplayCommand(INVOFF);
		toggle = 0;
	}
}

