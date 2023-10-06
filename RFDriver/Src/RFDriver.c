/**
  ******************************************************************************
  * @file     lib.c
  * @author   Auto-generated by STM32CubeIDE
  * @version  V1.0
  * @date     13/07/2023 16:54:02
  * @brief    Default under dev library file.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"
#include "RFDriver.h"
#include "RFNames.h"

/** Functions ----------------------------------------------------------------*/

static RF_HandleTypeDef hRF;

const uint8_t sizeOfPacketData = 32;
const uint8_t channel = 150;
const uint8_t sizeOfRXAddress = 5;
static uint8_t RXAddress[] = {0xEE,0xE7,0xE7,0xE7,0xEE};

static void nrf24_reset(uint8_t REG);

// GPIO FUNCTIONS

static void chipSelect(void)
{
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_RESET);
}

static void chipUnSelect(void)
{
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_SET);
}


static void chipEnable(void)
{
	HAL_GPIO_WritePin(hRF.portCE, hRF.pinCE, GPIO_PIN_SET);
}

static void chipDisable(void)
{
	HAL_GPIO_WritePin(hRF.portCE, hRF.pinCE, GPIO_PIN_RESET);
}


// Read and Writing Registers

static uint8_t readRegister(uint8_t address)
{
	// CSN Low
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_RESET);

	uint8_t data;
	uint8_t sizeOfData = 1;
	HAL_SPI_Transmit(hRF.hspi, &address, sizeOfData, HAL_MAX_DELAY);
	HAL_SPI_Receive(hRF.hspi, &data, sizeOfData, HAL_MAX_DELAY);

	// CSN High
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_SET);
	return data;
}

static void writeRegister(uint8_t address, uint8_t data)
{
	// CSN Low
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_RESET);

	uint8_t command = W_REGISTER | address;
	HAL_SPI_Transmit(hRF.hspi, &command, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hRF.hspi, &data, 1, HAL_MAX_DELAY);

	// CSN High
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_SET);
}



static void writeRegisterMultipleBytes(uint8_t address, uint8_t* data, uint8_t sizeOfData)
{
	// CSN Low
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_RESET);

	uint8_t command = W_REGISTER | address;
	HAL_SPI_Transmit(hRF.hspi, &command, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hRF.hspi, data, sizeOfData, HAL_MAX_DELAY);

	// CSN High
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_SET);
}

// Radio Functions

static void nrf24_reset(uint8_t REG)
{
	chipDisable();
	if (REG == STATUS)
	{
		writeRegister(STATUS, 0x00);
	}

	else if (REG == FIFO_STATUS)
	{
		writeRegister(FIFO_STATUS, 0x11);
	}

	else {
	writeRegister(CONFIG, 0x08);
	writeRegister(EN_AA, 0x3F);
	writeRegister(EN_RXADDR, 0x03);
	writeRegister(SETUP_AW, 0x03);
	writeRegister(SETUP_RETR, 0x03);
	writeRegister(RF_CH, 0x02);
	writeRegister(RF_SETUP, 0x0E);
	writeRegister(STATUS, 0x00);
	writeRegister(OBSERVE_TX, 0x00);
	writeRegister(CD, 0x00);
	uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	writeRegisterMultipleBytes(RX_ADDR_P0, rx_addr_p0_def, 5);
	uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
	writeRegisterMultipleBytes(RX_ADDR_P1, rx_addr_p1_def, 5);
	writeRegister(RX_ADDR_P2, 0xC3);
	writeRegister(RX_ADDR_P3, 0xC4);
	writeRegister(RX_ADDR_P4, 0xC5);
	writeRegister(RX_ADDR_P5, 0xC6);
	uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	writeRegisterMultipleBytes(TX_ADDR, tx_addr_def, 5);
	writeRegister(RX_PW_P0, 0);
	writeRegister(RX_PW_P1, 0);
	writeRegister(RX_PW_P2, 0);
	writeRegister(RX_PW_P3, 0);
	writeRegister(RX_PW_P4, 0);
	writeRegister(RX_PW_P5, 0);
	writeRegister(FIFO_STATUS, 0x11);
	writeRegister(DYNPD, 0);
	writeRegister(FEATURE, 0);
	}
}

static uint8_t isDataAvailable(void)
{
	return readRegister(STATUS) & RX_DR;
}

// pass struct at start of RF function
void RFSetup(RF_HandleTypeDef _hRF)
{
	hRF = _hRF;

	nrf24_reset(0); // Set to default values

	// Our custom values

	// Put into settings function

	//writeRegister(CONFIG, 0);  // will be configured later

	writeRegister(EN_AA, 0);  // No Auto ACK

	//writeRegister (EN_RXADDR, 0);  // Not Enabling any data pipe right now

	writeRegister(SETUP_AW, 0x03);  // 5 Bytes for the TX/RX address

	writeRegister(SETUP_RETR, 0);   // No retransmission

	//writeRegister(RF_CH, 0x02);

	writeRegister(RF_SETUP, 0x0E);   // Power= 0db, data rate = 2Mbps
}

void TXSetup(void)
{
	chipDisable();

	// set address
	writeRegisterMultipleBytes(TX_ADDR, RXAddress, sizeOfRXAddress);

	writeRegister(RF_CH, channel);  // select the channel

	// Put in transmit mode
	writeRegister(CONFIG, readRegister(CONFIG) & ~PRIM_RX);
	writeRegister(CONFIG, readRegister(CONFIG) | PWR_UP); // PWR up

	chipEnable();
}


static void writeTXPayload(uint8_t* data)
{
	// CSN Low

	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_RESET);

	uint8_t command = W_TX_PAYLOAD;
	HAL_SPI_Transmit(hRF.hspi, &command, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hRF.hspi, data, sizeOfPacketData, HAL_MAX_DELAY);

	// CSN High
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_SET);

	// Check data has been written?
}

static uint8_t TXBufferFull(void)
{
	return readRegister(FIFO_STATUS) & TX_FULL;
}

void TXSend(uint8_t* data)
{
	// CS Disable
	//chipDisable();
	while (TXBufferFull()) {}
	writeTXPayload(data);
	// CS Enable
	//chipEnable();
	//HAL_Delay(1);

	// CS Disable
	//chipDisable();
}


void RXSetup(void)
{
	chipDisable();
	nrf24_reset (STATUS);

	// set address
	writeRegisterMultipleBytes(RX_ADDR_P1, RXAddress, sizeOfRXAddress);

	writeRegister(RF_CH, channel);  // select the channel

	// set byte size
	writeRegister(RX_PW_P1, sizeOfPacketData);

	// select data pipe 2
	uint8_t en_rxaddr = readRegister(EN_RXADDR);
	en_rxaddr = en_rxaddr | (1<<1);
	writeRegister(EN_RXADDR, en_rxaddr);

	// Put in receive mode
	writeRegister(CONFIG, readRegister(CONFIG) | PRIM_RX | PWR_UP);
	chipEnable();
}

static void readRXPayload(uint8_t* data)
{
	// CSN Low
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_RESET);

	uint8_t command = R_RX_PAYLOAD;
	HAL_SPI_Transmit(hRF.hspi, &command, 1, 100);
	HAL_SPI_Receive(hRF.hspi, data, sizeOfPacketData, 1000);

	// CSN High
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_SET);
}

uint8_t RXReceive(uint8_t* data)
{
	// Receive One Packet // From pipeline 1
	if (!isDataAvailable())
	{
		return 0;
	}

	// Set Data Ready flag as stated in docs
	writeRegister(STATUS, readRegister(STATUS) | RX_DR);
	readRXPayload(data);
	return 1;
}

static uint8_t RXBufferEmpty(void)
{
	return readRegister(FIFO_STATUS) & RX_EMPTY;
}

// Test Code
uint32_t RXRecieveStream(uint8_t* data)
{
	if (!isDataAvailable())
	{
		return 0;
	}
	writeRegister(STATUS, readRegister(STATUS) | RX_DR);
	uint32_t count = 0;
	while (!RXBufferEmpty())
	{
		count++;
		readRXPayload(data);
	}
	return count;
}

void encodeData(uint8_t *data, uint16_t pot1, uint16_t pot2)
{
	uint8_t pot1_0 = pot1 & 0x00FF;
	uint8_t pot1_1 = pot1>>8;

	uint8_t pot2_0 = pot2 & 0x00FF;
	uint8_t pot2_1 = pot2>>8;

	data[0] = pot1_0;
	data[1] = pot1_1<<4 | pot2_1;
	data[2] = pot2_0;
}

void decodeData(uint8_t *data, uint16_t* pot1, uint16_t* pot2)
{
	*pot1 = data[0] | ((data[1]&0xF0)<<4);
	*pot2 = data[2] | ((data[1]&0x0F)<<8);
}

void encodeImage(uint8_t* data, uint16_t lineID, uint8_t* lineData)
{
	uint8_t lineID_0 = lineID & 0x00FF;
	uint8_t lineID_1 = lineID >> 8;
	memcpy(lineData, data + 2, sizeOfPacketData - 2);
	data[0] = lineID_0;
	data[1] = lineID_1;
}

void decodeImage(uint8_t* data, uint16_t* lineID, uint8_t* lineData)
{
	*lineID = data[0] | (data[1] << 8);
	lineData = data + 2;
}






/*
static void flushRXPayload(void)
{
	// CSN Low
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_RESET);

	uint8_t command = FLUSH_RX;
	HAL_SPI_Transmit(hRF.hspi, &command, 1, HAL_MAX_DELAY);

	// CSN High
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_SET);
}
*/

/*
static void flushTXPayload(void)
{
	// CSN Low
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_RESET);

	uint8_t command = FLUSH_TX;
	HAL_SPI_Transmit(hRF.hspi, &command, 1, HAL_MAX_DELAY);

	// CSN High
	HAL_GPIO_WritePin(hRF.portCSN, hRF.pinCSN, GPIO_PIN_SET);
}
*/
