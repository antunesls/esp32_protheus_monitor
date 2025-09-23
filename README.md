# 📟 ESP32 Protheus Monitor

A project using an ESP32 with an LCD display to monitor, in real-time, the **used licenses**, **logged-in users**, and **ERP resource consumption** of **TOTVS Protheus**.

> **Note:** This project is based on the **ESP32-S3** microcontroller, developed using the **PlatformIO** environment, and uses pins **8 (SDA)** and **9 (SCL)** to connect a **0.96" SSD1306 display**.

## 🧩 Overview

This project aims to provide a quick and practical way to visualize the operational status of the Protheus ERP system directly from a small IoT device. It is especially useful for system administrators and IT teams who want a physical dashboard showing key metrics without having to log into the server.

## 🔧 Features

- 📊 Display of current Protheus license usage
- 👥 Monitoring of active/logged-in users
- ⚙️ Overview of system resources (CPU, memory, or ERP-specific indicators)
- 🔄 Automatic refresh at configurable intervals
- 💡 Optional alerts (e.g. LCD color change or icon) when reaching critical thresholds

## 🛠️ Hardware Requirements

- ESP32-S3 Dev Board
- 0.96" SSD1306 I2C OLED Display (SDA: GPIO8, SCL: GPIO9)
- USB Cable for power/programming
- Optional: Buzzer or LED for alerts

## 💻 Software Requirements

- PlatformIO (recommended) or Arduino IDE
- ESP32 board libraries
- LCD I2C library (e.g., `LiquidCrystal_I2C`)
- Wi-Fi network credentials
- Access to a Protheus monitoring API or database endpoint

## 🗺️ Project Flow

You can view the project flow diagram on Miro: [ESP32 Protheus Monitor Flow](https://miro.com/app/board/uXjVIpw0MfU=/?share_link_id=585686759009)

## 📦 PlatformIO Dependencies

This project uses the following PlatformIO dependencies (see `platformio.ini`):

```
lib_deps = 
    tzapu/WiFiManager@^2.0.17
    adafruit/Adafruit GFX Library@^1.12.1
    adafruit/Adafruit SSD1306@^2.5.14
```

## 🔌 How It Works

1. The ESP32 connects to your Wi-Fi network.
2. It queries a REST API or database that exposes Protheus license/user/resource information.
3. The LCD displays the key data, cycling through different metrics every few seconds.

## 🚀 Getting Started

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/esp32-protheus-monitor.git
   ```
2. Open the project folder in [PlatformIO](https://platformio.org/install).
3. Make sure you have the ESP32-S3 board selected in your `platformio.ini`.
4. Connect your ESP32-S3 board to your computer.
5. Build and upload the firmware using PlatformIO (check the "Upload" button or run `pio run --target upload`).

For a detailed PlatformIO getting started guide, see: [PlatformIO Get Started](https://docs.platformio.org/en/latest/introduction/quickstart.html)
