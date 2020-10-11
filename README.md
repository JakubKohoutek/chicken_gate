# Chicken Gate

This repository contains circuit diagram and code for low power chicken door using
ESP-8266 (ESP-12) and RTC module.

## Description

Purpose of this project is to create a chicken door that will automatically open in the
given time in the morning and close in the given time in the evening.

## Circuit Diagram

![](circuit_diagram/image.png)

## Compilation and Upload Configuration

* Board: Generic ESP8266 Module
* Upload speed: 115200
* Flash size: 4M (1M SPIFFS)

## Adjustments To Minimize Power Consumption

I was able to lower the power consumption of ESP-8266 board by using the deep sleep mode.
The board is woken up by an RTC module or by an external switch, so no circuit is in place
(GPIO16 connected to RST) for ESP to be able to wake up itself.

Current draw of the board and supplemental circuit in the deep sleep mode is about ???.

## The Finished Product

TBD

## TODO

* Update circuit diagram
* Pre-fill timers in captive portal
* Timeout for wifi connection - long time of inactivity
* Persist information about open/closed gate
* Add sensors for open/close gate and do proper action