#include <driver/i2s.h>

#define I2S_WS  25   // IO25
#define I2S_SD  22   // SCL
#define I2S_SCK 26   // IO26
#define LED 17  // IO17

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 128,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, 16000, I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_MONO);
}

void loop() {
  int32_t sample = 0;
  size_t bytes_read;

  i2s_read(I2S_NUM_0, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);

  int16_t val = sample >> 14;
  Serial.print(val);
  Serial.print(",");
  Serial.print(5000);
  Serial.print(",");
  Serial.println(-5000);

  int output = abs(val) > 1500 ? 0 : 1;
  digitalWrite(LED, output);

  delay(1);
}
