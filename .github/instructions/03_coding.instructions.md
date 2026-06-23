---
name: "Abbott the Robot firmware coding instructions"
description: "Use when: working with Abbott the Robot firmware code"
applyTo: **/*
---

# Coding Standards

## Language Specifics
* **Standard:** C11
* **Types:** Always use `<stdint.h>` types (e.g., `uint32_t`, `int16_t`, `uint8_t`). Never use `int`, `long`, or `short`.
* **Constants:** Use `const` or `constexpr` instead of `#define` macros for typed constants.

## Safety & MISRA
* Pointers must be checked for `NULL` before dereferencing if passed from an external module.

## Error Handling
* Return `HAL_StatusTypeDef` or a custom `typedef enum` for driver function returns. Do not silently swallow errors.

## Documentation
* Use Doxygen-style comments for all public and non-public functions and data structures. Include parameter descriptions, return values, and any side effects.
