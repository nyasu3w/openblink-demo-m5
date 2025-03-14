# OpenBlink Demo M5

[![English](https://img.shields.io/badge/language-English-blue.svg)](README.md)
[![中文](https://img.shields.io/badge/language-中文-red.svg)](README.zh-CN.md)
[![日本語](https://img.shields.io/badge/language-日本語-green.svg)](README.ja.md)

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

## Documentation

For more detailed documentation, please check the [doc directory](./doc)

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
- The `break if Blink.req_reload?` statement is crucial in OpenBlink applications. It allows the program to gracefully exit the current execution loop when a code reload is requested through the Bluetooth interface. Without this check, the program would continue running and ignore reload requests, making development and debugging difficult. This mechanism is what enables the "Blink" feature - the ability to update code wirelessly in less than 0.1 seconds without restarting the microprocessor.
