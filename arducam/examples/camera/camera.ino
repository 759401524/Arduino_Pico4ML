#include "arducam.h"

uint8_t image_data[96 * 96];
struct arducam_config config;
uint8_t header[2] = {0x55, 0xAA};

void setup() {
  Serial1.begin(921600);
  pinMode(LED_BUILTIN, OUTPUT);
  // put your setup code here, to run once:
  config.sccb            = i2c0;
  config.sccb_mode       = I2C_MODE_16_8;
  config.sensor_address  = 0x24;
  config.pin_sioc        = PIN_CAM_SIOC;
  config.pin_siod        = PIN_CAM_SIOD;
  config.pin_resetb      = PIN_CAM_RESETB;
  config.pin_xclk        = PIN_CAM_XCLK;
  config.pin_vsync       = PIN_CAM_VSYNC;
  config.pin_y2_pio_base = PIN_CAM_Y2_PIO_BASE;
  config.pio             = pio0;
  config.pio_sm          = 0;
  config.dma_channel     = 0;
  arducam_init(&config);

}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  arducam_capture_frame(&config, (uint8_t *)image_data);
  uint8_t *displayBuf = new uint8_t[96 * 96 * 2];
  uint16_t index      = 0;
  for (int x = 0; x < 96 * 96; x++) {
    uint16_t imageRGB   = ST7735_COLOR565(image_data[x], image_data[x], image_data[x]);
    displayBuf[index++] = (uint8_t)(imageRGB >> 8) & 0xFF;
    displayBuf[index++] = (uint8_t)(imageRGB) & 0xFF;
  }
  delete[] displayBuf;

  uart_write_blocking(UART_ID, header, 2);
  uart_write_blocking(UART_ID, (uint8_t *)image_data, kMaxImageSize);
  
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}
