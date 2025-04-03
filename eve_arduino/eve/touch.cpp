#include "eve.h"

int16_t last_touch_x = -32768;
int16_t last_touch_y = -32768;
TaskHandle_t touchTaskHandle = NULL;
bool handling_touch = false;
TouchCallback touch_callback = NULL; 

void IRAM_ATTR handle_touch_interrupt() {
  if (handling_touch) {
    return;
  }
  handling_touch = true;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
  // Notify the touch handling task
  if (touchTaskHandle != NULL) {
    vTaskNotifyGiveFromISR(touchTaskHandle, &xHigherPriorityTaskWoken);
  }
  
  // If a higher priority task was woken, request a context switch
  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();
  }
}

bool init_touch_interrupts(int interrupt_pin, TouchCallback callback) {
  // Store the callback function
  touch_callback = callback;

  // 1. Enable the TOUCH interrupt flag in the mask// Configure touch mode to ensure touch is active
  wr8(RAM_REG + REG_TOUCH_MODE, 3); // Continuous mode
  
  // Configure interrupt for touch
  // Clear any pending interrupts
  rd8(RAM_REG + REG_INT_FLAGS);
  
  // Set touch interrupt mask
  wr8(RAM_REG + REG_INT_MASK, 0x02);  // Enable TOUCH flag only (bit 1)
  
  // Enable global interrupt
  wr8(RAM_REG + REG_INT_EN, 0x01);    // Enable global interrupt
  
  // Configure INT_N as push-pull output
  uint16_t gpiox = rd16(RAM_REG + REG_GPIOX);
  
  // Configure INT_N as push-pull output (set bit 7)
  gpiox |= (1 << 7);
  wr16(RAM_REG + REG_GPIOX, gpiox);
  
  // Setup hardware interrupt pin
  pinMode(interrupt_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), handle_touch_interrupt, CHANGE);
  Serial.print("Attached EVE interrupt to pin ");
  Serial.println(interrupt_pin);

  // Create touch handling task
  xTaskCreatePinnedToCore(
    touchHandlerTask,    // Task function
    "TouchHandler",      // Name for debugging
    2048*2,              // Stack size (adjust as needed)
    NULL,                // Parameters
    1,                   // Priority
    &touchTaskHandle,    // Task handle
    1                    // Run on Core 1 (Application core)
  );
  
  return true;
}

void touchHandlerTask(void* parameter) {
  for (;;) {
    // Wait for notification from ISR
    uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    if (notification > 0) {
      // Read and clear interrupt flags
      uint8_t flags = rd8(RAM_REG + REG_INT_FLAGS);
      Serial.printf("Touch interrupt detected! Flags: 0x%02X\n", flags);
      
      // Read touch coordinates and update last_touch_x/y
      read_touch_coordinates();
      
      // Call the callback function with touch coordinates if it exists
      if (touch_callback != NULL) {
        touch_callback(last_touch_x, last_touch_y);
      }
    }
    handling_touch = false;
  }
}

// Function to read and store touch coordinates
void read_touch_coordinates() {
  // Read touch coordinates
  uint32_t touchXY = rd32(RAM_REG + REG_TOUCH_SCREEN_XY);
  last_touch_x = touchXY >> 16;
  last_touch_y = touchXY & 0xFFFF;
  
  Serial.print("Touch X: ");
  Serial.print(last_touch_x);
  Serial.print(", Y: ");
  Serial.println(last_touch_y);
}