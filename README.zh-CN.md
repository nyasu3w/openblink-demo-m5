# OpenBlink Demo M5 (中文)

[![English](https://img.shields.io/badge/language-English-blue.svg)](README.md)
[![中文](https://img.shields.io/badge/language-中文-red.svg)](README.zh-CN.md)
[![日本語](https://img.shields.io/badge/language-日本語-green.svg)](README.ja.md)

## 什么是 OpenBlink

**_OpenBlink_** 是一个从 **_ViXion Blink_** 分支出来的开源项目。

- 可以使用 Ruby 这种高效轻量级语言来开发嵌入式设备。
- 程序重写和调试控制台完全无线。（蓝牙低功耗）
- 重写时间少于 0.1 秒，且不涉及微处理器重启。（我们称之为"Blink"）

## 如何获取 OpenBlink

要克隆仓库并初始化子模块，请运行以下命令：

```console
$ git clone https://github.com/OpenBlink/openblink-demo-m5.git
$ pio run
$ pio run -t erase && pio run -t upload
```

## 已验证的硬件

以下硬件平台已经过 OpenBlink 测试：

- M5 StampS3 (Espressif ESP32-S3FN8)

## 文档

有关更详细的文档，请查看[文档目录](./doc)

## mruby/c LED 控制 API

OpenBlink 提供了一个简单的 API，通过 mruby/c 控制板载 RGB LED。

### 可用的类和方法

#### LED 类

- `LED.set([r, g, b])` - 设置 RGB LED 颜色。每个值应在 0-255 之间。

#### Blink 类

- `Blink.req_reload?` - 检查是否请求代码重载。

### 示例：LED 闪烁代码

这是一个使 LED 以不同颜色闪烁的简单示例：

```ruby
# RGB LED 闪烁示例
while true do
  # 红色
  LED.set([255, 0, 0])
  sleep 1

  # 绿色
  LED.set([0, 255, 0])
  sleep 1

  # 蓝色
  LED.set([0, 0, 255])
  sleep 1

  # 检查是否请求重载
  break if Blink.req_reload?
end
```

此示例演示了：

- 使用`LED.set`方法设置 RGB LED 颜色
- 使用数组指定 RGB 值
- 在请求代码重载时实现干净退出
- `break if Blink.req_reload?` 语句在 OpenBlink 应用程序中至关重要。它允许程序在通过蓝牙接口请求代码重载时优雅地退出当前执行循环。如果没有这个检查，程序将继续运行并忽略重载请求，使开发和调试变得困难。这种机制正是实现"Blink"功能的关键 - 能够在不重启微处理器的情况下，以不到 0.1 秒的时间无线更新代码。
