#include "stm32f4xx_hal.h"

#include "display.h"
#include "displayNames.h"

static Display_HandleTypeDef hDisplay;

// GPIO Functions
#define LCD_2IN_Write_Command sendDisplayCommand


static void sendDisplayCommand(uint8_t command);
static void sendDisplayData(uint8_t* pData, uint16_t sizeOfData);


void LCD_2IN_WriteData_Byte(uint8_t data) {
	sendDisplayData(&data, 1);
}

void LCD_2IN_Init(void)
{
	//LCD_2IN_Reset();

	LCD_2IN_Write_Command(0x36);
	LCD_2IN_WriteData_Byte(0x00);

	LCD_2IN_Write_Command(0x3A);
	LCD_2IN_WriteData_Byte(0x05);

	LCD_2IN_Write_Command(0x21);

	LCD_2IN_Write_Command(0x2A);
	LCD_2IN_WriteData_Byte(0x00);
	LCD_2IN_WriteData_Byte(0x00);
	LCD_2IN_WriteData_Byte(0x01);
	LCD_2IN_WriteData_Byte(0x3F);

	LCD_2IN_Write_Command(0x2B);
	LCD_2IN_WriteData_Byte(0x00);
	LCD_2IN_WriteData_Byte(0x00);
	LCD_2IN_WriteData_Byte(0x00);
	LCD_2IN_WriteData_Byte(0xEF);

	LCD_2IN_Write_Command(0xB2);
	LCD_2IN_WriteData_Byte(0x0C);
	LCD_2IN_WriteData_Byte(0x0C);
	LCD_2IN_WriteData_Byte(0x00);
	LCD_2IN_WriteData_Byte(0x33);
	LCD_2IN_WriteData_Byte(0x33);

	LCD_2IN_Write_Command(0xB7);
	LCD_2IN_WriteData_Byte(0x35);

	LCD_2IN_Write_Command(0xBB);
	LCD_2IN_WriteData_Byte(0x1F);

	LCD_2IN_Write_Command(0xC0);
	LCD_2IN_WriteData_Byte(0x2C);

	LCD_2IN_Write_Command(0xC2);
	LCD_2IN_WriteData_Byte(0x01);

	LCD_2IN_Write_Command(0xC3);
	LCD_2IN_WriteData_Byte(0x12);

	LCD_2IN_Write_Command(0xC4);
	LCD_2IN_WriteData_Byte(0x20);

	LCD_2IN_Write_Command(0xC6);
	LCD_2IN_WriteData_Byte(0x0F);

	LCD_2IN_Write_Command(0xD0);
	LCD_2IN_WriteData_Byte(0xA4);
	LCD_2IN_WriteData_Byte(0xA1);

	LCD_2IN_Write_Command(0xE0);
	LCD_2IN_WriteData_Byte(0xD0);
	LCD_2IN_WriteData_Byte(0x08);
	LCD_2IN_WriteData_Byte(0x11);
	LCD_2IN_WriteData_Byte(0x08);
	LCD_2IN_WriteData_Byte(0x0C);
	LCD_2IN_WriteData_Byte(0x15);
	LCD_2IN_WriteData_Byte(0x39);
	LCD_2IN_WriteData_Byte(0x33);
	LCD_2IN_WriteData_Byte(0x50);
	LCD_2IN_WriteData_Byte(0x36);
	LCD_2IN_WriteData_Byte(0x13);
	LCD_2IN_WriteData_Byte(0x14);
	LCD_2IN_WriteData_Byte(0x29);
	LCD_2IN_WriteData_Byte(0x2D);

	LCD_2IN_Write_Command(0xE1);
	LCD_2IN_WriteData_Byte(0xD0);
	LCD_2IN_WriteData_Byte(0x08);
	LCD_2IN_WriteData_Byte(0x10);
	LCD_2IN_WriteData_Byte(0x08);
	LCD_2IN_WriteData_Byte(0x06);
	LCD_2IN_WriteData_Byte(0x06);
	LCD_2IN_WriteData_Byte(0x39);
	LCD_2IN_WriteData_Byte(0x44);
	LCD_2IN_WriteData_Byte(0x51);
	LCD_2IN_WriteData_Byte(0x0B);
	LCD_2IN_WriteData_Byte(0x16);
	LCD_2IN_WriteData_Byte(0x14);
	LCD_2IN_WriteData_Byte(0x2F);
	LCD_2IN_WriteData_Byte(0x31);
	LCD_2IN_Write_Command(0x21);

	LCD_2IN_Write_Command(0x11);

	LCD_2IN_Write_Command(0x29);
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

static uint8_t receiveDisplayDataByte(void) {
	uint8_t byteData;
	HAL_GPIO_WritePin(hDisplay.portDC, hDisplay.pinDC, GPIO_PIN_SET);
	HAL_GPIO_WritePin(hDisplay.portCS, hDisplay.pinCS, GPIO_PIN_RESET);
	HAL_SPI_Receive(hDisplay.hspi, &byteData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(hDisplay.portCS, hDisplay.pinCS, GPIO_PIN_SET);
	return byteData;
}

static void receiveDisplayData(uint8_t* pData, uint16_t sizeOfData) {
	HAL_GPIO_WritePin(hDisplay.portDC, hDisplay.pinDC, GPIO_PIN_SET);
	HAL_GPIO_WritePin(hDisplay.portCS, hDisplay.pinCS, GPIO_PIN_RESET);
	HAL_SPI_Receive(hDisplay.hspi, pData, sizeOfData, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(hDisplay.portCS, hDisplay.pinCS, GPIO_PIN_SET);
}

static uint16_t receiveDisplayDataWord(void) {
	uint8_t wordSplit[2];
	receiveDisplayData(wordSplit, 2);
	uint16_t word = (wordSplit[0] << 8) + wordSplit[1];
	return word;
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

uint16_t getScanLine(void) {
	sendDisplayCommand(GSCAN);
	(void) receiveDisplayDataByte();
	return receiveDisplayDataWord();
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
	static toggle = 0;
	if (toggle == 0) {
		sendDisplayCommand(INVON);
		toggle = 1;
	} else {
		sendDisplayCommand(INVOFF);
		toggle = 0;
	}
}

