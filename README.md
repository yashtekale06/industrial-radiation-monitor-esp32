# ⚡ ESP32 RS-485 Modbus Radiation Sensor

A lightweight and efficient ESP32 firmware to read **radiation (W/m²)** from a **Modbus RTU sensor** over **RS-485**, using **low-level GPIO register control** for fast and reliable DE/RE switching.

---

## ✨ Features

- 📡 Modbus RTU (Function Code `0x03`)
- ⚙️ Direct ESP32 GPIO register access (fast RS-485 control)
- 🔐 CRC-16 validation (Modbus standard)
- ⏱ Timeout, CRC, and frame error diagnostics
- 🧪 Deterministic and stable communication
- 🧩 Minimal dependencies (Arduino framework only)

---

## 🧠 How It Works

1. ESP32 sends a Modbus request frame  
2. RS-485 transceiver switches to **TX mode**
3. Sensor responds with radiation data
4. ESP32 switches back to **RX mode**
5. Frame is validated (Slave ID, Function, Length)
6. CRC is verified
7. Radiation value is extracted and displayed

---

## 🔌 Hardware Required

- ESP32 Dev Board  
- RS-485 Transceiver (MAX485 / SP3485)  
- Modbus Radiation Sensor  
- External power supply (as per sensor specs)

---

## 🔗 Wiring

| RS-485 Module | ESP32 |
|--------------|-------|
| RO           | RX    |
| DI           | TX    |
| DE + RE     | GPIO 14 |
| VCC          | 3.3V / 5V |
| GND          | GND   |

---

## 📡 Communication Settings

- **Protocol:** Modbus RTU  
- **Baud Rate:** 4800  
- **Data Bits:** 8  
- **Parity:** None  
- **Stop Bits:** 1  
- **Slave ID:** `0x01`

---

## 📊 Serial Output Example

- RAD=520 W/m2  CRC_ERR=0 TIMEOUT_ERR=0 FRAME_ERR=0


---

## 🛠 Development Environment

- **Framework:** Arduino (ESP32)
- **Build System:** PlatformIO
- **Monitor Speed:** 4800 baud

---

## 🚀 Possible Improvements

- Use `Serial2` for dedicated RS-485 UART
- Add Modbus retry mechanism
- FreeRTOS task separation
- MQTT / ThingSpeak / Cloud integration
- ESP-IDF version for production systems

---

## 📜 License

This project is licensed under the **MIT License**.  
Free to use, modify, and distribute.

---

## 👨‍💻 Author

Designed for **embedded systems learning, academic projects, and industrial Modbus applications**.


