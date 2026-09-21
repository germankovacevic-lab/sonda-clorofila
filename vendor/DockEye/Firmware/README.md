# Instructions
Simply flash the code to your ESP32-S3 using Arduino IDE after downloading the required libraries (see below). I am using an Adafruit ESP32-S3 Feather which uses the ESP32-S3 MINI-1 SOC, other versions of the ESP32-S3 are currently untested. When powered, the code will automatically run and measurements will be taken. The instrument will then power down automatically, and results will be written to a file called Test.csv on the SD card.

# Required Libraries
To download the necessary libraries, simply open the library manager on the Arduino IDE and search for and download the following:
- Adafruit AS7341 by Adafruit
- Adafruit AW9523 by Adafruit
- RTClib by Adafruit
- SdFat - Adafruit Fork by Bill Greiman
