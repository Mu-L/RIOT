@defgroup    boards_common_slwstk6000b Silicon Labs SLWSTK6000B starter kit
@ingroup     boards
@brief       Support for the Silicon Labs SLWSTK6000B starter kit

## Overview
Silicon Labs Mighty Gecko Wireless Starter Kit is equipped with the EFM32
microcontroller. It is specifically designed for low-power applications, having
energy-saving peripherals, different energy modes and short wake-up times.

The starter kit is equipped with an Advanced Energy Monitor. This allows you to
actively measure the power consumption of your hardware and code, in real-time.

## Hardware

### MCU
The MCU depends on the module used.

| Module     | MCU                     |
|------------|-------------------------|
| SLWRB4150A | EFR32MG1P233F256GM48    |
| SLWRB4150B | EFR32MG1P233F256GM48    |
| SLWRB4151A | EFR32MG1P232F256GM48    |
| SLWRB4152A | EFR32MG1P232F256GM48    |
| SLWRB4153A | EFR32MG1P132F256GM48    |
| SLWRB4154A | EFR32MG1P732F256GM32    |
| SLWRB4158A | EFR32MG13P733F512GM48   |
| SLWRB4159A | EFR32MG13P632F512GM48   |
| SLWRB4161A | EFR32MG12P432F1024GL125 |
| SLWRB4162A | EFR32MG12P332F1024GL125 |
| SLWRB4163A | EFR32MG12P433F1024GL125 |
| SLWRB4164A | EFR32MG12P433F1024GL125 |
| SLWRB4167A | EFR32MG13P733F512GM48   |
| SLWRB4168A | EFR32MG13P732F512GM48   |
| SLWRB4169A | EFR32MG14P733F256GM48   |
| SLWRB4169B | EFR32MG14P733F256GM48   |
| SLWRB4170A | EFR32MG12P433F1024GM68  |

**Note:** not all MCUs are supported by RIOT-OS out of the box.

## Implementation Status
| Device           | ID                    | Supported | Comments                                                       |
|------------------|-----------------------|-----------|----------------------------------------------------------------|
| MCU              | EFR32MG1P/12P/13P/14P | yes       | Power modes supported                                          |
| Low-level driver | ADC                   | yes       |                                                                |
|                  | Flash                 | yes       |                                                                |
|                  | GPIO                  | yes       | Interrupts are shared across pins (see reference manual)       |
|                  | HW Crypto             | yes       |                                                                |
|                  | I2C                   | yes       |                                                                |
|                  | PWM                   | yes       |                                                                |
|                  | RTCC                  | yes       | As RTT or RTC                                                  |
|                  | SPI                   | partially | Only master mode                                               |
|                  | Timer                 | yes       |                                                                |
|                  | UART                  | yes       | USART is shared with SPI. LEUART baud rate limited (see below) |
|                  | USB                   | no        |                                                                |

## Board configuration

### Board controller
The starter kit is equipped with a Board Controller. This controller provides a
virtual serial port. The boardcontroller is enabled via a GPIO pin.

By default, this pin is enabled. You can disable the board controller module by
passing `DISABLE_MODULE=silabs_bc` to the `make` command.

**Note:** to use the virtual serial port, ensure you have the latest board
controller firmware installed.

**Note:** the board controller *always* configures the virtual serial port at
115200 baud with 8 bits, no parity and one stop bit. This also means that it
expects data from the MCU with the same settings.

### Advanced Energy Monitor
This development kit has an Advanced Energy Monitor. It can be connected to the
Simplicity Studio development software.

This development kit can measure energy consumption and correlate this with the
code. It allows you to measure energy consumption on code-level.

The board controller is responsible for measuring energy consumption. For
real-time code correlation, the CoreDebug peripheral will be configured to
output MCU register data and interrupt data via the SWO port.

By default, this feature is enabled. It can be disabled by passing
`DISABLE_MODULE=silabs_aem` to the `make` command.

Note that Simplicity Studio requires debug symbols to correlate code. RIOT-OS
defaults to GDB debug symbols, but Simplicity Studio requires DWARF-2 debug
symbols (`-gdwarf-2` for GCC).

### Clock selection
There are several clock sources that are available for the different
peripherals. You are advised to read
[AN0004.1](https://www.silabs.com/documents/public/application-notes/an0004.1-efm32-cmu.pdf)
to get familiar with the different clocks.

| Source | Internal | Speed      | Comments                           |
|--------|----------|------------|------------------------------------|
| HFRCO  | Yes      | 19 MHz     | Enabled during startup, changeable |
| HFXO   | No       | 38.4 MHz   |                                    |
| LFRCO  | Yes      | 32.768 kHz |                                    |
| LFXO   | No       | 32.768 kHz |                                    |
| ULFRCO | No       | 1 kHz      | Not very reliable as a time source |

The sources can be used to clock following branches:

| Branch | Sources                 | Comments                     |
|--------|-------------------------|------------------------------|
| HF     | HFRCO, HFXO             | Core, peripherals            |
| LFA    | LFRCO, LFXO             | Low-power timers             |
| LFB    | LFRCO, LFXO, CORELEDIV2 | Low-power UART               |
| LFE    | LFRCO, LFXO             | Real-time Clock and Calendar |

CORELEDIV2 is a source that depends on the clock source that powers the core.
It is divided by 2 or 4 to not exceed maximum clock frequencies (EMLIB takes
care of this).

The frequencies mentioned in the tables above are specific for this starter
kit.

It is important that the clock speeds are known to the code, for proper
calculations of speeds and baud rates. If the HFXO or LFXO are different from
the speeds above, ensure to pass `EFM32_HFXO_FREQ=freq_in_hz` and
`EFM32_LFXO_FREQ=freq_in_hz` to your compiler.

You can override the branch's clock source by adding `CLOCK_LFA=source` to your
compiler defines, e.g. `CLOCK_LFA=cmuSelect_LFRCO`.

### Low-power peripherals
The low-power UART is capable of providing an UART peripheral using a low-speed
clock. When the LFB clock source is the LFRCO or LFXO, it can still be used in
EM2. However, this limits the baud rate to 9600 baud. If a higher baud rate is
desired, set the clock source to CORELEDIV2.

**Note:** peripheral mappings in your board definitions will not be affected by
this setting. Ensure you do not refer to any low-power peripherals.

### RTC or RTT
RIOT-OS has support for *Real-Time Tickers* and *Real-Time Clocks*.

However, this board MCU family has support for a 32-bit *Real-Time Clock and
Calendar*, which can be configured in ticker mode **or** calendar mode.
Therefore, only one of both peripherals can be enabled at the same time.

Configured at 1 Hz interval, the RTCC will overflow each 136 years.

### Hardware crypto
This MCU is equipped with a hardware accelerated crypto peripheral that can
speed up AES128, AES256, SHA1, SHA256 and several other cryptographic
computations.

A peripheral driver interface for RIOT-OS is proposed, but not yet implemented.

### Usage of EMLIB
This port makes uses of EMLIB by Silicon Labs to abstract peripheral registers.
While some overhead is to be expected, it ensures proper setup of devices,
provides chip errata and simplifies development. The exact overhead depends on
the application and peripheral usage, but the largest overhead is expected
during peripheral setup. A lot of read/write/get/set methods are implemented as
inline methods or macros (which have no overhead).

Another advantage of EMLIB are the included assertions. These assertions ensure
that peripherals are used properly. To enable this, pass `DEBUG_EFM` to your
compiler.

### Pin locations
The EFM32 platform supports peripherals to be mapped to different pins
(predefined locations). The definitions in `periph_conf.h` mostly consist of a
location number and the actual pins. The actual pins are required to configure
the pins via GPIO driver, while the location is used to map the peripheral to
these pins.

In other words, these definitions must match. Refer to the data sheet for more
information.

This MCU has extended pin mapping support. Each pin of a peripheral can be
connected separately to one of the predefined pins for that peripheral.

## Flashing the device
To flash, [SEGGER JLink](https://www.segger.com/jlink-software.html) is
required.

Flashing is supported by RIOT-OS using the command below:

```
make flash
```

To run the GDB debugger, use the command:

```
make debug
```

Or, to connect with your own debugger:

```
make debug-server
```

Some boards have (limited) support for emulation, which can be started with:

```
make emulate
```

## Supported Toolchains
For using the Silicon Labs SLWSTK6000B starter kit we strongly recommend the
usage of the [GNU Tools for ARM Embedded Processors](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm)
toolchain.

## License information
Silicon Labs' EMLIB: zlib-style license (permits distribution of source).
