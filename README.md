# OpenBlink Demo M5

_Towards a programmable world that changes in the blink of an eye._

## What is OpenBlink

**_OpenBlink_** is an open source project forked from **_ViXion Blink_**.

- Ruby, a highly productive lightweight language, can be used to develop embedded devices.
- Program rewriting and debugging console are completely wireless. (BluetoothLE)
- Rewriting time is less than 0.1 second and does not involve a microprocessor restart. (We call it "Blink".)

## How to Get OpenBlink

To clone the repository and initialize the submodules, run the following commands:

```console
$ git clone https://github.com/OpenBlink/openblink-demo-m5.git
$ pio run
$ pio run -t erase && pio run -t upload
```

## Verified Hardware

The following hardware platforms have been tested with OpenBlink:

- M5 StampS3 (Espressif ESP32-S3FN8)

## Development Environment Versions

- PlatformIO Core, version 6.1.16

```console
openblink-demo-m5 $ pio pkg list
Resolving m5stack-stamps3 dependencies...
Platform espressif32 @ 6.10.0 (required: espressif32 @ 6.10.0)
├── framework-arduinoespressif32 @ 3.20017.241212+sha.dcc1105b (required: platformio/framework-arduinoespressif32 @ ~3.20017.0)
├── framework-espidf @ 3.50400.0 (required: platformio/framework-espidf @ ~3.50400.0)
├── tool-cmake @ 3.16.4 (required: platformio/tool-cmake @ ~3.16.0)
├── tool-esptoolpy @ 1.40501.0 (required: platformio/tool-esptoolpy @ ~1.40501.0)
├── tool-mkfatfs @ 2.0.1 (required: platformio/tool-mkfatfs @ ~2.0.0)
├── tool-mklittlefs @ 1.203.210628 (required: platformio/tool-mklittlefs @ ~1.203.0)
├── tool-mkspiffs @ 2.230.0 (required: platformio/tool-mkspiffs @ ~2.230.0)
├── tool-ninja @ 1.9.0 (required: platformio/tool-ninja @ ^1.7.0)
├── tool-openocd-esp32 @ 2.1100.20220706 (required: platformio/tool-openocd-esp32 @ ~2.1100.0)
├── tool-riscv32-esp-elf-gdb @ 12.1.0+20221002 (required: espressif/tool-riscv32-esp-elf-gdb @ ~12.1.0)
├── tool-xtensa-esp-elf-gdb @ 12.1.0+20221002 (required: espressif/tool-xtensa-esp-elf-gdb @ ~12.1.0)
├── toolchain-esp32ulp @ 1.23800.240113 (required: platformio/toolchain-esp32ulp @ ~1.23800.0)
├── toolchain-riscv32-esp @ 14.2.0+20241119 (required: platformio/toolchain-riscv32-esp @ 14.2.0+20241119)
└── toolchain-xtensa-esp-elf @ 14.2.0+20241119 (required: platformio/toolchain-xtensa-esp-elf @ 14.2.0+20241119)

Libraries
├── M5Unified @ 0.2.3 (required: m5stack/M5Unified @ 0.2.3)
│   └── M5GFX @ 0.2.5 (required: M5GFX @ >=0.2.4)
└── mrubyc @ 0.0.0+20250216090831.sha.2cbbbf7 (required: git+https://github.com/mrubyc/mrubyc.git#2cbbbf757bbc9366fd319dd76753dc2c8b8386b9)
```

## mruby/c LED Control API

OpenBlink provides a simple API for controlling the onboard RGB LED through mruby/c.

### Available Classes and Methods

#### LED Class

- `LED.set([r, g, b])` - Sets the RGB LED color. Each value should be between 0-255.

#### Blink Class

- `Blink.req_reload?` - Checks if a code reload is requested.

### Example: LED Blinking Code

Here's a simple example that makes the LED blink in different colors:

```ruby
# RGB LED Blinking Example
while true do
  # Red
  LED.set([255, 0, 0])
  sleep 1

  # Green
  LED.set([0, 255, 0])
  sleep 1

  # Blue
  LED.set([0, 0, 255])
  sleep 1

  # Check if reload is requested
  break if Blink.req_reload?
end
```

This example demonstrates:

- Setting RGB LED colors using the `LED.set` method
- Using arrays to specify RGB values
- Implementing a clean exit when code reload is requested

### Example2: LCD output code
This is just an additional code on the previous LED Blinking Code.
```
RED = [255, 0, 0]
GREEN = [0, 255, 0]
BLUE = [0, 0, 255]

while true do
  # Red
  Display.set_text_color(63488);
  Display.puts "      RED"
  puts "RED"
  LED.set([255, 0, 0])
  sleep 1
  
  # Green
  Display.set_text_color(2016);
  Display.puts "      GREEN"
  puts "GREEN"
  LED.set([0, 255, 0])
  sleep 1
  
  # Blue
  Display.set_text_color(31);
  Display.puts "      BLUE"
  puts "BLUE"
  LED.set([0, 0, 255])
  sleep 1
  
  # Check if reload is requested
  break if Blink.req_reload?
end
```
