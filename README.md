# 📟 ESP32 Protheus Monitor

A project using an ESP32 with an LCD display to monitor, in real-time, the **used licenses**, **logged-in users**, and **ERP resource consumption** of **TOTVS Protheus**.

## 🧩 Overview

This project aims to provide a quick and practical way to visualize the operational status of the Protheus ERP system directly from a small IoT device. It is especially useful for system administrators and IT teams who want a physical dashboard showing key metrics without having to log into the server.

## 🔧 Features

- 📊 Display of current Protheus license usage
- 👥 Monitoring of active/logged-in users
- ⚙️ Overview of system resources (CPU, memory, or ERP-specific indicators)
- 🔄 Automatic refresh at configurable intervals
- 💡 Optional alerts (e.g. LCD color change or icon) when reaching critical thresholds

## 🛠️ Hardware Requirements

- ESP32 Dev Board
- I2C LCD Display (e.g., 16x2 or 20x4)
- USB Cable for power/programming
- Optional: Buzzer or LED for alerts

## 💻 Software Requirements

- Arduino IDE or PlatformIO
- ESP32 board libraries
- LCD I2C library (e.g., `LiquidCrystal_I2C`)
- Wi-Fi network credentials
- Access to a Protheus monitoring API or database endpoint

## 🔌 How It Works

1. The ESP32 connects to your Wi-Fi network.
2. It queries a REST API or database that exposes Protheus license/user/resource information.
3. The LCD displays the key data, cycling through different metrics every few seconds.

## 🚀 Getting Started

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/esp32-protheus-monitor.git
