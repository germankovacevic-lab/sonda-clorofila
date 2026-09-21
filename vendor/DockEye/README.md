# DockEye

## About

This is the repository for an in-development, open-source, low-cost multispectral sensor for measuring water quality parameters. The current objective is measuring chlorophyll-*a* and turbidity,
though more parameters may be added in the future. This sensor uses a cuvette-based approach, similar to benchtop spectrofluorometers, rather than a truly in-situ one. The advantage of this approach is the ability to control ambient light while not strictly needing to be waterproof. Samples can be collected from the surface by hand or at depth using a Van Dorn sampler or CTD-Rosette.

This sensor continues the research from the now-deprecated https://github.com/COAST-Lab/Low-Cost-Multiparameter-Water-Quality-Sensor built using the Particle Argon microcontroller. 
The new version uses an ESP32-S3, which uses the Arduino IDE rather than the Particle Workbench.

The MOORE Foundation's SOCON (Sustained Ocean Color Observations from Nanosatellites) program funds this project.

## Introduction

This sensor is built using an Adafruit ESP32-S3 Feather microcontroller, an Adafruit Adalogger/RTC, an AS7341 light sensor, an AW9523 LED driver, a 430 nm blue LED, a 910 nm NIR LED, and a 3D printed enclosure. 
This repository currently contains firmware for flashing to the microcontroller and 3D printable design models for the enclosure, and scripts for data analysis, etc. This sensor is in active development, and this repository will be updated continuously as progress is made. 

## Parts

A list of parts can be found here:

- [ESP32-S3 Microcontroller](https://www.adafruit.com/product/5477)
- [Adalogger Datalogger/RTC](https://www.adafruit.com/product/2922)
- [AW9523 LED Driver](https://www.adafruit.com/product/4886)
- [AS7341 Light Sensor](https://www.adafruit.com/product/4698)
- [Blue LED](https://www.thorlabs.com/thorproduct.cfm?partnumber=LED430L)
- [NIR LED](https://www.thorlabs.com/thorproduct.cfm?partnumber=LED910L)

Additional parts include a soldering iron, solder, wire, a breadboard, access to a 3D printer, an SD card, a USB-C data cable, and hardware for assembling the sensor. 

A glass or quartz cuvette will be needed for calibrating and running samples. Because the chlorophyll-*a* calibration procedure is completed in acetone, plastic cuvettes are not advised. Acetone can dissolve some plastics, although certain plastics may be incompatible. The glass cuvettes that we are currently using are rather inexpensive, and can be found on Amazon [here](https://www.amazon.com/Fluorescence-Spectrophotometer-Cuvettes-pathlength-Fluorometer/dp/B07YDZ6K2B?crid=TBT16WYR38HM&dib=eyJ2IjoiMSJ9.Rx6TjeI6e-pFg3Mv68xtz28ZP_Q9hPI-yeQDbd4sN2l0LnT0KrBOkWjchOp21-8up7uFRnGXH4NQ6EaVXD32cu2CRUydlVmxSTyXF9Sp9oGG_GkgRuUTdxTE_vuw7iFTRlXKrGBJVSP3u1wZ12FmtyhuNQ71NkWIS9Gg2aIDsoQEjo3d2ZUMWSBvYHaW4YlxplcTskGwRPYe4TKxq8esrUAnyXS-2K3-_TNK5aQQeNM.WDhcMW94lNpOqJgwk2HZfrtGYfncKdvNy5RRSyoP6e8&dib_tag=se&keywords=glass+cuvettes&qid=1747334719&sprefix=glass+cuvettes%2Caps%2C126&sr=8-8). Ensure that any cuvettes purchased are specifically for fluorometry (with four transparent sides) and not for spectrophotometers, as those only have two transparent sides. These are incompatible with this design, as the LEDs in this design are placed at a 90-degree angle from the light detector. 

3D print files can be found [here](https://github.com/COAST-Lab/Open-Fluorometer/tree/main/Hardware/Housing).

## Wiring Instructions

Instructions for assembly will be added soon. For now, here is a simple explanation on how to wire the parts together:

- Solder the included male headers onto the corresponding pins on the ESP-32
- Solder the stacking headers onto corresponding pins on the Adalogger
- Solder/insert (depending on type) pins into breadboard
- Add a jumper wire from 3.3V and GND to +/-
- Solder the following pins on the AW9523 to the corresponding pins on the ESP32-S3
  - VIN to +
  - GND to -
  - SCL to SCL
  - SDA to SDA
- Solder the following pins on the AS7341 to the corresponding pins on the ESP32-S3
  - VIN to +
  - GND to -
  - SCL to SCL
  - SDA to SDA
- Solder the Blue LED to + and AW9523 Pin 10
- Solder the NIR LED to + and AW9523 Pin 0

Instructions for mounting the LEDs and light sensor to the cuvette holder will be added soon.

## Data Analysis

Scripts for data analysis and data files can be found [here](https://github.com/COAST-Lab/Open-Fluorometer/tree/main/Data%20Analysis).
  

