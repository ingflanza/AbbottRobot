---
name: "Abbott the Robot firmware architecture instructions"
description: "Use when: working with Abbott the Robot firmware architecture, including task management, driver layering, and system design"
applyTo: **/*
---

# Firmware Architecture

## Operating System
* **Environment:** Bare-metal superloop

## Concurrency and Tasks
* **Tasks:** Simple one call tasks can be created. For more complex tasks, state machines can be implemented using a state variable and a switch statement. Avoid using FreeRTOS or other RTOS in this project.
* **ISRs (Interrupt Service Routines):** ISRs must be as short as possible. Use `...FromISR` API variants exclusively inside callbacks. Defer heavy processing to a task using a notification flag or similar pattern. Avoid delays or blocking calls inside ISRs.

## Memory Management
* **Dynamic Memory Allocation:** Avoid using dynamic memory allocation (e.g., `malloc`, `free`) in the firmware. Use static memory allocation for all data structures and buffers to ensure deterministic behavior and avoid fragmentation, unless it is explicitly instructed.

## Driver Layering
* **Hardware Abstraction:** We use STM32 HAL for complex peripherals and STM32 HAL for hardware resources direct access.
* **Functionality Abstraction:** All hardware drivers must be wrapped in a higher-level driver that provides a simplified interface to the application logic layer For example, an IMU driver should provide functions like `IMU_Init()`, `IMU_ReadData()`, etc., instead of exposing low-level HAL calls directly.
