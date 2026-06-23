# FreeTest — STM32F446RE UART-Controlled LEDs

An **STM32F446RE** project that controls two LEDs' blink rate via UART commands. It uses custom register-level drivers (GPIO HAL, interrupt-driven UART) and FreeRTOS, with the UART driver supporting both bare-metal and RTOS builds.


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

### UART4 Wiring

| Nucleo Pin | Function | Connect to |
|---|---|---|
| PA0 | UART4_TX | adapter RX |
| PA1 | UART4_RX | adapter TX |
| GND | — | adapter GND |

### LEDs

| Pin | LED |
|---|---|
| PA5 | LED1 (onboard LD2) |
| PA4 | LED2 (external) |

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

## Build

The project supports both bare-metal and FreeRTOS builds, selected by the `make` target:

- `make` — build the FreeRTOS version
- `make RTOS=0` — build the bare-metal version

To build and flash:

- `make flash` — flash the FreeRTOS version
- `make RTOS=0 flash` — flash the bare-metal version

Both options exist because the UART driver supports both build types, and the target context must be specified at compile time. The same mechanism will apply to future drivers that support both builds.


---

## Usage

Open a serial terminal at 115200 baud and send a command in one of these formats:

- `A<rate>` — set LED1's blink rate
- `B<rate>` — set LED2's blink rate

`<rate>` is the blink period in milliseconds. Press Enter to apply the new rate.

**Example:** `A500` sets LED1 to blink every 500 ms.
**Example:** `B300` sets LED2 to blink every 300 ms.

---

## Author

**Cristopher Reyes Ramírez** — Communications and Electronics Engineer
[github.com/creyes2001](https://github.com/creyes2001)
