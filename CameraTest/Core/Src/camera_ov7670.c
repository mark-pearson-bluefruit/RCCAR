#include "stm32l4xx_hal.h"
#include "camera_ov7670.h"


static GPIO_TypeDef* GPIO_VS;
static uint16_t GPIO_Pin_VS;
static GPIO_TypeDef* GPIO_HS;
static uint16_t GPIO_Pin_HS;
static GPIO_TypeDef* GPIO_RST;
static uint16_t GPIO_Pin_RST;
static GPIO_TypeDef* GPIO_PWNN;
static uint16_t GPIO_Pin_PWNN;
static GPIO_TypeDef* GPIO_PCLK;
static uint16_t GPIO_Pin_PCLK;

static struct dataPin* dataPins;

void resetCamera(void) {
	HAL_GPIO_WritePin(GPIO_RST, GPIO_Pin_RST, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIO_RST, GPIO_Pin_RST, GPIO_PIN_SET);
	HAL_Delay(300);
}

void turnOnCamera(void) {
	HAL_GPIO_WritePin(GPIO_PWNN, GPIO_Pin_PWNN, GPIO_PIN_RESET);
	HAL_Delay(200);
}

void startUp() {
	resetCamera();
	turnOnCamera();
}

void CAMSetup(GPIO_TypeDef *_GPIO_VS, uint16_t _GPIO_Pin_VS,
		      GPIO_TypeDef *_GPIO_HS, uint16_t _GPIO_Pin_HS,
			  GPIO_TypeDef *_GPIO_RST, uint16_t _GPIO_Pin_RST,
			  GPIO_TypeDef *_GPIO_PWNN, uint16_t _GPIO_Pin_PWNN,
			  GPIO_TypeDef* _GPIO_PCLK, uint16_t _GPIO_Pin_PCLK, struct dataPin* _dataPins) {
	GPIO_VS = _GPIO_VS;
	GPIO_Pin_VS = _GPIO_Pin_VS;
	GPIO_HS = _GPIO_HS;
	GPIO_Pin_HS = _GPIO_Pin_HS;
	GPIO_RST = _GPIO_RST;
	GPIO_Pin_RST = _GPIO_Pin_RST;
	GPIO_PWNN = _GPIO_PWNN;
	GPIO_Pin_PWNN = _GPIO_Pin_PWNN;
	GPIO_PCLK = _GPIO_PCLK;
	GPIO_Pin_PCLK = _GPIO_Pin_PCLK;

	dataPins = _dataPins;

	startUp();
}

void getFrame(uint8_t* pixelData, int sizeOfBuffer) {

	// Wait until VS goes from high to low (means start of new frame)
	// Wait until VS is high
	for (int i = 0; i < sizeOfBuffer; i++) {
		pixelData[i] = 0;
	}

	while(!HAL_GPIO_ReadPin(GPIO_VS, GPIO_Pin_VS));
	// Wait until VS is low
	while(HAL_GPIO_ReadPin(GPIO_VS, GPIO_Pin_VS));

	// Only read when HS is high
	for (int i = 0; i < sizeOfBuffer; i++) {
		// Wait until HS is high
		while(!HAL_GPIO_ReadPin(GPIO_HS, GPIO_Pin_HS));

		// Wait until pixel clock goes from low to high
		// Wait until pixel clock is low
		while(HAL_GPIO_ReadPin(GPIO_PCLK, GPIO_Pin_PCLK));
		// Wait until pixel clock is high
		while(!HAL_GPIO_ReadPin(GPIO_PCLK, GPIO_Pin_PCLK));
		// Read data to buffer
		const int numOfDataLines = 4;
		for (int dataLine = 0; dataLine < numOfDataLines; dataLine++) {
			pixelData[i] += HAL_GPIO_ReadPin(dataPins[dataLine].port, dataPins[dataLine].pin)<<(dataLine);
		}
		pixelData[i] += 48;
	}
}

