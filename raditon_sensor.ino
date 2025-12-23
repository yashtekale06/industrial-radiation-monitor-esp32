#include <Arduino.h>

#define RE_DE_GPIO 14

#define GPIO_OUT_W1TS    (*(volatile uint32_t *)0x60000304)
#define GPIO_OUT_W1TC    (*(volatile uint32_t *)0x60000308)
#define GPIO_ENABLE_W1TS (*(volatile uint32_t *)0x6000030C)

static const uint8_t modbus_req[8] = {
  0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A
};

static volatile uint16_t radiation_value = 0;
static volatile uint32_t crc_error_cnt = 0;
static volatile uint32_t timeout_error_cnt = 0;
static volatile uint32_t frame_error_cnt = 0;

uint16_t CRC16(uint8_t *buf, uint8_t len) {
  uint16_t crc = 0xFFFF;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= buf[i];
    for (uint8_t b = 0; b < 8; b++) {
      crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : (crc >> 1);
    }
  }
  return (crc << 8) | (crc >> 8);
}

inline void rs485_tx_enable() {
  GPIO_OUT_W1TS = (1 << RE_DE_GPIO);
}

inline void rs485_rx_enable() {
  GPIO_OUT_W1TC = (1 << RE_DE_GPIO);
}

void rs485_send_frame(const uint8_t *buf, uint8_t len) {
  rs485_tx_enable();
  delayMicroseconds(1500);
  for (uint8_t i = 0; i < len; i++) {
    Serial.write(buf[i]);
  }
  Serial.flush();
  delayMicroseconds(1500);
  rs485_rx_enable();
}

bool rs485_receive_frame(uint8_t *buf, uint8_t expected_len, uint32_t timeout_ms) {
  uint8_t idx = 0;
  uint32_t start = millis();
  while ((millis() - start) < timeout_ms) {
    while (Serial.available()) {
      buf[idx++] = Serial.read();
      if (idx >= expected_len) return true;
    }
  }
  return false;
}

void modbus_read_radiation() {
  uint8_t rx[7];

  rs485_send_frame(modbus_req, 8);

  if (!rs485_receive_frame(rx, 7, 500)) {
    timeout_error_cnt++;
    return;
  }

  if (rx[0] != 0x01 || rx[1] != 0x03 || rx[2] != 0x02) {
    frame_error_cnt++;
    return;
  }

  uint16_t crc_calc = CRC16(rx, 5);
  uint16_t crc_rx = ((uint16_t)rx[5] << 8) | rx[6];

  if (crc_calc != crc_rx) {
    crc_error_cnt++;
    return;
  }

  radiation_value = ((uint16_t)rx[3] << 8) | rx[4];
}

void diagnostics_print() {
  Serial.print("RAD=");
  Serial.print(radiation_value);
  Serial.print(" W/m2  CRC_ERR=");
  Serial.print(crc_error_cnt);
  Serial.print(" TIMEOUT_ERR=");
  Serial.print(timeout_error_cnt);
  Serial.print(" FRAME_ERR=");
  Serial.println(frame_error_cnt);
}

void setup() {
  Serial.begin(4800);
  GPIO_ENABLE_W1TS = (1 << RE_DE_GPIO);
  rs485_rx_enable();
}

void loop() {
  modbus_read_radiation();
  diagnostics_print();
  delay(2000);
}
