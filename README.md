# FreeTest — STM32F446RE Bare-metal HAL + FreeRTOS

Bare-metal firmware project for the **STM32F446RE Nucleo-64** board, built entirely in C without an IDE.
The goal is to build a clean, layered HAL from scratch while integrating FreeRTOS, mimicking the
architectural discipline expected in professional embedded software development.

> **Status:** Active development — UART4 interrupt-driven TX/RX complete, next step: FreeRTOS integration.

---

## Hardware

| Item | Details |
|---|---|
| Board | STM32 Nucleo-64 (STM32F446RE) |
| Core | ARM Cortex-M4F @ 180 MHz |
| Clock source | HSE 8 MHz (ST-Link oscillator, bypass mode) |
| SYSCLK | 180 MHz via PLL (PLLM=8, PLLN=360, PLLP=2) |
| PCLK1 (APB1) | 45 MHz |
| PCLK2 (APB2) | 90 MHz |

---
## Toolchain

| Tool | Purpose |
|---|---|
| `arm-none-eabi-gcc` | Cross-compiler |
| `OpenOCD` | Flashing and debugging via ST-Link |
| `GDB` | Hardware-level debugging |
| `Make` | Build system |
| Linux | Development environment |

No IDE. All compilation, flashing, and debugging done from the command line.

---

## Project Structure

```
FreeTest/
├── inc/                      # All header files
├── src/                      # All source files (HAL drivers + application)
├── submodules/               # Git submodules (FreeRTOS)
├── build/                    # Compiled output (gitignored)
├── main.c
├── Makefile
└── README.md
```

---


## HAL Modules

### Clock — RCC Configuration
- HSE bypass mode enabled (required for Nucleo ST-Link oscillator)
- Over-Drive mode enabled to reach 180 MHz (PWR requirement on F446)
- Full PLL configuration: PLLM=8 / PLLN=360 / PLLP=2
- Flash latency set to 5 wait states per datasheet requirement
- AHB, APB1, APB2 prescalers configured explicitly

### GPIO
- Alternate function configuration via MODER + AFRL/AFRH registers
- Bit manipulation uses correct `pin * 2` shifts (not `2^pin` XOR — documented bug fix)
- Supports input, output, and AF modes

### UART4
- Peripheral on APB1 (45 MHz) — BRR calculated correctly for this bus
- TX and RX both interrupt-driven
- Ring buffer on TX path
- Verified at 115200 baud

---

## FreeRTOS — Next Step

> The repository is named after this integration target — FreeRTOS is not yet in the codebase.

Once UART4 RX is complete, the project transitions to FreeRTOS integration:

- Port: `ARM_CM4F` (hardware FPU support)
- Will be added as a **Git submodule** under `submodules/`
- Existing HAL drivers will remain unchanged — FreeRTOS sits on top of the bare-metal layer

---

## Key Bugs Resolved

These are documented because the debugging process is part of the learning:

| Bug | Root Cause | Fix |
|---|---|---|
| GPIO MODER misconfiguration | Used `2^pin` (XOR) instead of `pin * 2` (left shift) | Corrected bit shift in MODER write macro |
| HSE clock not starting | Nucleo uses ST-Link oscillator — requires HSE bypass mode, not crystal mode | Enabled `RCC_CR_HSEBYP` before `HSEON` |
| 180 MHz PLL unstable | Over-Drive mode not enabled; F446 requires it above 168 MHz | Added PWR Over-Drive enable + wait sequence |
| UART4 wrong baud rate | BRR calculated using SYSCLK (180 MHz) instead of PCLK1 (45 MHz) | Fixed BRR formula to use correct APB1 clock |

---

## Build & Flash

```bash
# Build
make

# Flash via OpenOCD
make flash

# Debug session
make debug
```

> OpenOCD config: `board/st_nucleo_f4.cfg`

---

## Goals / Roadmap

- [x] PLL clock tree to 180 MHz
- [x] GPIO HAL with alternate function support
- [x] UART4 TX at 115200 baud
- [x] Interrupt-driven UART4 TX with ring buffer
- [x] UART4 RX with interrupt handling
- [ ] FreeRTOS integration (ARM_CM4F port)

---

## Why This Project Exists

This is a deliberate portfolio project to build the skills required for **automotive embedded software** roles.
The architecture follows the layered approach (HAL → Driver → Application) common in AUTOSAR Classic environments,
implemented from scratch to understand the fundamentals before working with generated code and BSW stacks.

Every driver is written bare-metal against the RM0390 reference manual — no STM32 HAL library, no CubeMX.

---

## Author

**Cristopher Reyes Ramírez** — Communications and Electronics Engineer
[github.com/creyes2001](https://github.com/creyes2001)
