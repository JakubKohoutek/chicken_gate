# Chicken Gate

This repository contains circuit diagram and code for low power chicken door using
ESP-8266 (ESP-12) and DS3231 RTC module.

## Description

Purpose of this project is to create a chicken door that will automatically open in the
given time in the morning and close in the given time in the evening.

Besides this automatic procedures, manual operation is also allowed using external button
to open or close gate whenever needed.

Configuration and time setting is possible via long pressing the external button - that
will start a WiFi access point with captive portal, so once user connects to the
`chicken-gate` WiFi, he is redirected to the settings page, where it is possible to
synchronize time or set a different timer for opening/closing the gate.

## Circuit Diagram

![](circuit_diagram/diagram_image.png)

## Compilation and Upload Configuration

* Board: Generic ESP8266 Module
* Upload speed: 115200
* Flash size: 4M (1M SPIFFS)

## Adjustments To Minimize Power Consumption

I was able to lower the power consumption of ESP-8266 board by using the deep sleep mode.
The board is woken up by an RTC module or by an external signal, so no circuit is in place
(GPIO16 connection to RST pin) for ESP to be able to wake up itself.

Current draw of the board and supplemental circuit in the deep sleep mode is about 27 uA
(~0.03 mA) and approximately 700 mA when opening/closing the gate (while stepper motor
is responsible for most of it). One operation cycle (closing, opening) takes in my case
34 seconds.

It means that in theory two operation cycles per day will drain 13.3 mAh while sleeping for
the rest of the day will drain additional 0.72 mAh. One standard day will therefore need
around 14 mAh.

It means that one 18650 battery with capacity of 1800 mAh should power the device for around
128 days, i.e. over 4 months.

## The Finished Product

![Open Gate](./images/open_gate.jpg)
![Closed Gate](./images/closed_gate.jpg)
To see the video of working device, check out the [video on YouTube](https://youtu.be/cAucfu40Rm4).

## TODO

* Update circuit diagram
* Pre-fill timers in captive portal (nice to have)
* Create 3D models for electronics housing
* Add stl files for printed parts