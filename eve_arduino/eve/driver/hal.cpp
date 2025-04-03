#include <stdint.h>       // Find types like "uint8_t"
#include <stdbool.h>      // Find type "bool"
#include <SPI.h>          // Arduino SPI library
#include <SD.h>           // Arduino SD card library

#include "hw_api.h"
#include "Arduino.h"

#define SPI_SPEED 20000000  // 20 MHz (20,000,000 Hz)

// Global variables to store pin assignments
static uint8_t _eveChipSelectPin = 0;
static uint8_t _evePdnPin = 0;

// Initialization function to set pin assignments
void HAL_Eve_Init(uint8_t chipSelectPin, uint8_t pdnPin)
{
  _eveChipSelectPin = chipSelectPin;
  _evePdnPin = pdnPin;
  
  // Configure pins
  pinMode(_eveChipSelectPin, OUTPUT);
  pinMode(_evePdnPin, OUTPUT);
  
  // Initialize default states
  digitalWrite(_eveChipSelectPin, HIGH);
  digitalWrite(_evePdnPin, HIGH);
}

void HAL_Delay(uint32_t milliSeconds)
{
  delay(milliSeconds);
}

uint8_t HAL_SPI_WriteByte(uint8_t data)
{
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
  digitalWrite(_eveChipSelectPin, LOW);

  SPI.transfer(data);
      
  digitalWrite(_eveChipSelectPin, HIGH);
  SPI.endTransaction();
  return 0;
}

// Send a series of bytes (contents of a buffer) through SPI
void HAL_SPI_WriteBuffer(uint8_t *Buffer, uint32_t Length)
{
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
  digitalWrite(_eveChipSelectPin, LOW);

  SPI.transfer(Buffer, Length);
      
  digitalWrite(_eveChipSelectPin, HIGH);
  SPI.endTransaction();
}

// Send a byte through SPI as part of a larger transmission.  Does not enable/disable SPI CS
uint8_t HAL_SPI_Write(uint8_t data)
{
//  Log("W-0x%02x\n", data);
  SPI.transfer(data);
  return 0;
}

// Read a series of bytes from SPI and store them in a buffer
void HAL_SPI_ReadBuffer(uint8_t *Buffer, uint32_t Length)
{
  uint8_t a = SPI.transfer(0x00); // dummy read

  while (Length--)
  {
    *(Buffer++) = SPI.transfer(0x00);
  }
}

// Enable SPI by activating chip select line
void HAL_SPI_Enable(void)
{
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
  digitalWrite(_eveChipSelectPin, LOW);
}

// Disable SPI by deasserting the chip select line
void HAL_SPI_Disable(void)
{
  digitalWrite(_eveChipSelectPin, HIGH);
  SPI.endTransaction();
}

void HAL_Eve_Reset_HW(void)
{
  // Reset Eve
  digitalWrite(_evePdnPin, 0);                    // Set the Eve PDN pin low
  HAL_Delay(50);                             // delay
  digitalWrite(_evePdnPin, 1);                    // Set the Eve PDN pin high
  HAL_Delay(100);                            // delay
}

// Required for EVE library
extern "C" {
  void DBG(const char* data) {
    Serial.println(data);
  }
}
