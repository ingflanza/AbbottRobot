---
name: "Abbott the Robot Hardware instructions"
description: "Use when: working with Abbott the Robot hardware, including sensors, actuators, and communication interfaces"
applyTo: **/*
---

# Hardware Context

## Microcontroller
* **Family:** STM32 Family of microcontrollers
* **Board:** Nucleo-F411RE
* **Part Number:** STM32F411RET6
* **Core:** ARM Cortex-M4
* **Clock Speed:** 84 MHz

## Memory Map Constraints
* **Flash:** 512 Kbytes of flash memory
* **RAM:** 128 Kbytes of SRAM

## Peripheral Setup (CubeMX Managed)
* Do not reconfigure peripheral GPIOs or Clocks manually in user code; assume `main.c` initialization handles this via CubeMX generated code.
* Do not change or propose any changes to ioc project file.
* **USART3:** Used for Debug logging using standard printf functionality. Baud Rate: 115200 (8N1).
* **ADC1 CHANNEL 1:** Used for IR TCRT5000 Line sensor 1. PA6. Right side sensor.
* **ADC1 CHANNEL 6:** Used for IR TCRT5000 Line sensor 2. PA1. Left side sensor.
* **TIM3 CHANNEL 3:** Used for PWM output to control the MOTOR1. PB0. Right side motor.
* **TIM3 CHANNEL 2:** Used for PWM output to control the MOTOR2. PA7. Left side motor.
* **TIM2 INPUT CAPTURE CHANNEL 1:** Used for reading Ultrasonic sensor echo signal pulse width. PA0.
* **PA0:** Configured temporarily as GPIO output to trigger the Ultrasonic sensor. After triggering, it is reconfigured back to TIM2 input capture mode.
