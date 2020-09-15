# Chicken Gate

This repository contains circuit diagram and code for low power chicken door using
ESP-8266 (ESP-12) and RTC module.

## Description

Purpose of this project is to create a chicken door that will automatically open in the
given time in the morning and close in the given time in the evening.

## Circuit Diagram

TBD

## Credentials and Configuration

This snippet uses a configuration file `config.h` that stores credentials and confidential
information used within the code.

To create the file, run the following script (replace `***` with the real values):

```bash
cat << EOF >> config.h
#ifndef CONFIG_H
#define CONFIG_H

#define SOME_CONSTANT "***"

#endif
EOF
```

## Compilation and Upload Configuration

* Board: Generic ESP8266 Module
* Upload speed: 115200
* Flash size: 4M (1M SPIFFS)

## Adjustments To Minimize Power Consumption

I was able to lower the power consumption of ESP-8266 board by using the deep sleep mode. I
am using the maximum reliable deep sleep interval of 3 hours.

Current draw of the board and supplemental circuit in the deep sleep mode is about ???.

## The Finished Product

TBD