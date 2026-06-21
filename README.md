# 🤖 Abbott the Robot

![Abbott the Robot](abbott-the-robot.jpg)

Welcome to the official firmware repository for **Abbott the Robot**, an educational and hobbyist mobile robot platform! Abbott is designed to be an accessible, 3D-printable, and highly customizable introduction to embedded systems and robotics. 

This repository contains the C/C++ firmware built around the powerful ARM Cortex-M4 **STM32F411** microcontroller, utilizing the STM32 HAL (Hardware Abstraction Layer).

---

## ✨ Features

Abbott comes packed with features perfect for learning autonomous navigation, sensor integration, and motor control:

* **Differential Drive System:** Independent control of two DC gear motors for precise steering and speed regulation via PWM.
* **Obstacle Avoidance:** Integrated HC-SR04 Ultrasonic distance sensor for scanning the environment and preventing collisions.
* **Line Following Capabilities:** Two downward-facing IR sensors for track navigation and edge detection.
* **Audio Feedback:** A piezo buzzer programmed to play simple melodies and emit status alerts (e.g., startup sequence, error states).
* **Visual Indicators:** LED lights for debugging and aesthetic feedback.

---

## 🛠️ Hardware Requirements

To build and run Abbott, you will need the following core components:

* **Microcontroller:** STM32F411 Development Board (e.g., "Black Pill")
* **Motor Driver:** Dual DC Motor Driver (e.g., TB6612FNG, DRV8833, or L298N)
* **Actuators:** 2x DC Gear Motors with custom/standard wheels
* **Sensors:**
    * 1x HC-SR04 Ultrasonic Distance Sensor
    * 2x IR Line Tracking Sensors (e.g., TCRT5000 modules)
* **Feedback:** Piezo buzzer (passive), standard 5mm LEDs
* **Power:** LiPo battery (e.g., 2S 7.4V) with a 5V buck converter/voltage regulator to power the STM32.

---

## 📍 Configuration

*Note: Comming*

| Component | STM32 Pin | Function / Peripheral |
| :--- | :--- | :--- |
| **Feature** | `PA0` | GPIO Output |

---

## 💻 Software Setup & Installation

This project is configured using **STM32CubeIDE**. 

### Prerequisites
1. Download and install [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html).
2. You will need an ST-Link V2 (or compatible) programmer to flash the board.

### Building the Firmware
1. **Clone the repository:**
   ```bash
   git clone [https://github.com/yourusername/abbott-the-robot.git](https://github.com/yourusername/abbott-the-robot.git)