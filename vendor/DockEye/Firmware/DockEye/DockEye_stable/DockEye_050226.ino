#include <Adafruit_AS7341.h>
#include <Adafruit_AW9523.h>
#include <RTClib.h>
#include <SdFat.h>

Adafruit_AS7341 as7341;
Adafruit_AW9523 aw;
RTC_PCF8523 rtc;
File myFile;

float sumChl_F8 = 0;
int countChl_F8 = 0;

float sumTurb_NIR = 0;
int countTurb_NIR = 0;

float sumChl_BG = 0;
int countChl_BG = 0;

float sumTurb_BG = 0;
int countTurb_BG = 0;

float calibrated_chla = 0;
float calibrated_turb = 0;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const int SD_CHIP_SELECT = 10;

const uint8_t LedPins[] = {0, 1, 3, 10, 5};
const int NumLEDs = sizeof(LedPins) / sizeof(LedPins[0]);

int LEDbright = 255;
int LEDdim = 0;

char filename[24]; 

SdFat SD;

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    delay(1);
  }

  if (!as7341.begin(0x39)) {
    Serial.println("Could not find AS7341");
    while (1) { delay(10); }
  }

  as7341.setATIME(255);
  as7341.setASTEP(999);
  as7341.setGain(AS7341_GAIN_256X);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT running, setting time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!aw.begin(0x58)) {
    Serial.println("AW9523 not found? Check wiring!");
    while (1) delay(10);
  }

  Serial.println("AW9523 found!");

  for (int i = 0; i < NumLEDs; i++) {
    aw.pinMode(LedPins[i], AW9523_LED_MODE);
  }

  SD.begin(SD_CHIP_SELECT);

  DateTime now = rtc.now();
  sprintf(filename, "Test %04d-%02d-%02d.csv", now.year(), now.month(), now.day());

  File file = SD.open(filename, FILE_WRITE);
  if (file) {
    file.println("Date and Time,ASTEP,ATIME,GAIN,LED Status,LED Intensity,Test,Seconds,Milliseconds,Type,F1 (Basic),F2 (Basic),F3 (Basic),F4 (Basic),F5 (Basic),F6 (Basic),F7 (Basic),F8 (Basic),Clear (Basic),NIR (Basic),Type,F1 (Raw),F2 (Raw),F3 (Raw),F4 (Raw),F5 (Raw),F6 (Raw),F7 (Raw),F8 (Raw),Clear (Raw),NIR (Raw),Measurement Type");
    file.close();
  } else {
    Serial.println(F("Error opening file!"));
  }
}

void loop() {
  sumChl_F8 = sumChl_BG = sumTurb_NIR = sumTurb_BG = 0;
  countChl_F8 = countChl_BG = countTurb_NIR = countTurb_BG = 0;

  Serial.println("Begin background measurements.");
  takeMeasurements(60000, "Background");
  Serial.println("End background measurements.");

  Serial.println("Begin chlorophyll measurements.");
  turnOnLED(3, 60000, "Chlorophyll-a");
  Serial.println("End chlorophyll measurements.");

  Serial.println("Begin turbidity measurements.");
  turnOnLED(0, 60000, "Turbidity");
  Serial.println("End turbidity measurements.");

  aw.analogWrite(LedPins[3], LEDdim);
  aw.analogWrite(LedPins[2], LEDbright);

  // final summary
  if (countChl_F8 > 0 && countChl_BG > 0 && countTurb_NIR > 0 && countTurb_BG > 0) {
    // chlorophyll-a calculations
    float avgChl_F8 = sumChl_F8 / countChl_F8;
    float avgChl_BG = sumChl_BG / countChl_BG;
    float chlDiff = avgChl_F8 - avgChl_BG;

    // turbidity calculations
    float avgTurb_NIR = sumTurb_NIR / countTurb_NIR;
    float avgTurb_BG = sumTurb_BG / countTurb_BG;
    float turbDiff = avgTurb_NIR - avgTurb_BG;

    // calibration equations
    float calibrated_chla = 0.0871 * chlDiff - 1.3054;
    float calibrated_turb = 0.0081 * turbDiff - 11.787;

    // log summary to SD
    File file = SD.open(filename, FILE_WRITE);
    if (file) {
      file.println("SUMMARY");
      file.print("Average F8 Raw Counts,");
      file.println(avgChl_F8);
      file.print("Average F8 Background,");
      file.println(avgChl_BG);
      file.print("Measured F8 - BG,");
      file.println(chlDiff);
      file.print("Calibrated Chlorophyll-a,");
      file.println(calibrated_chla);
      file.print("Average NIR Raw Counts,");
      file.println(avgTurb_NIR);
      file.print("Average NIR Background,");
      file.println(avgTurb_BG);
      file.print("Measured NIR - BG,");
      file.println(turbDiff);
      file.print("Calibrated Turbidity (NTU),");
      file.println(calibrated_turb);
      file.close();
    }

    // print to serial
    Serial.println("==== FINAL AVERAGE RESULTS ====");
    Serial.print("F8 Avg. Raw Counts: ");
    Serial.println(avgChl_F8);
    Serial.print("Avg. Background Counts: ");
    Serial.println(avgChl_BG);
    Serial.print("Background Subtracted F8 Counts: ");
    Serial.println(chlDiff);
    Serial.print("Calibrated Chlorophyll-a (μg/L): ");
    Serial.println(calibrated_chla);
    Serial.println("================================");
    Serial.print("NIR Avg. Raw Counts: ");
    Serial.println(avgTurb_NIR);
    Serial.print("Avg. Background Counts: ");
    Serial.println(avgTurb_BG);
    Serial.print("Background Subtracted NIR Counts: ");
    Serial.println(turbDiff);
    Serial.print("Calibrated Turbidity (NTU): ");
    Serial.println(calibrated_turb);
    Serial.println("================================");
  }

  while (1) {
    delay(1);
  }
}

void takeMeasurements(unsigned long duration, const char* measurementType) {
  for (int i = 0; i < NumLEDs; i++) {
    aw.analogWrite(LedPins[i], LEDdim);
  }

  unsigned long startTime = millis();
  unsigned long lastProgressTime = 0;
  const int barWidth = 30;

  while (millis() - startTime < duration) {
    uint16_t readings[12];
    if (!as7341.readAllChannels(readings)) {
      Serial.println("Error reading all channels!");
      continue;
    }

    fileprintresults(SD, rtc, measurementType, readings);
    serialprintresults(rtc, measurementType, readings);

    unsigned long now = millis();
    if (now - lastProgressTime >= 1000) {
      float percent = (float)(now - startTime) / duration;
      if (percent > 1.0) percent = 1.0;
      int hashes = percent * barWidth;

      Serial.print(measurementType);
      Serial.print(": [");
      for (int i = 0; i < barWidth; ++i) {
        if (i < hashes) Serial.print("#");
        else Serial.print(" ");
      }
      Serial.print("] ");
      Serial.print((int)(percent * 100));
      Serial.print("%   ");
      Serial.println();
      lastProgressTime = now;
    }
  }
}

void turnOnLED(uint8_t ledIndex, unsigned long duration, const char* measurementType) {
  unsigned long startTime = millis();
  unsigned long lastProgressTime = 0;
  const int barWidth = 30;

  while (millis() - startTime < duration) {
    for (int i = 0; i < NumLEDs; i++) {
      if (i == ledIndex) {
        aw.analogWrite(LedPins[i], LEDbright);
      } else {
        aw.analogWrite(LedPins[i], LEDdim);
      }
    }

    uint16_t readings[12];
    if (!as7341.readAllChannels(readings)) {
      Serial.println("Error reading all channels!");
      continue;
    }

    fileprintresults(SD, rtc, measurementType, readings);
    serialprintresults(rtc, measurementType, readings);

    unsigned long now = millis();
    if (now - lastProgressTime >= 1000) {
      float percent = (float)(now - startTime) / duration;
      if (percent > 1.0) percent = 1.0;
      int hashes = percent * barWidth;

      Serial.print(measurementType);
      Serial.print(": [");
      for (int i = 0; i < barWidth; ++i) {
        if (i < hashes) Serial.print("#");
        else Serial.print(" ");
      }
      Serial.print("] ");
      Serial.print((int)(percent * 100));
      Serial.print("%   ");
      Serial.println();
      lastProgressTime = now;
    }
  }
}

void fileprintresults(SdFat &sd, RTC_PCF8523 &rtc, const char* measurementType, uint16_t readings[]) {
  File file = sd.open(filename, FILE_WRITE);
  DateTime now = rtc.now();

  float counts[12];

  for (uint8_t i = 0; i < 12; i++) {
    if (i == 4 || i == 5) continue;
    counts[i] = as7341.toBasicCounts(readings[i]);
  }

  if (file) {
    file.print(now.year(), DEC);
    file.print('/');
    file.print(now.month(), DEC);
    file.print('/');
    file.print(now.day(), DEC);
    file.print(" (");
    file.print(daysOfTheWeek[now.dayOfTheWeek()]);
    file.print(") ");
    file.print(now.hour(), DEC);
    file.print(':');
    file.print(now.minute(), DEC);
    file.print(':');
    file.print(now.second(), DEC);
    file.print(',');
    file.print(as7341.getASTEP());
    file.print(',');
    file.print(as7341.getATIME());
    file.print(',');
    file.print(as7341.getGain());
    file.print(',');
    file.print("LED Status");
    file.print(',');
    file.print("LED Intensity");
    file.print(',');
    file.print("6.0");
    file.print(',');
    file.print(millis() / 1000);
    file.print(',');
    file.print(millis());
    file.print(',');
    file.print("BASIC COUNTS");
    file.print(',');
    file.print(counts[0]);
    file.print(',');
    file.print(counts[1]);
    file.print(',');
    file.print(counts[2]);
    file.print(',');
    file.print(counts[3]);
    file.print(',');
    file.print(counts[6]);
    file.print(',');
    file.print(counts[7]);
    file.print(',');
    file.print(counts[8]);
    file.print(',');
    file.print(counts[9]);
    file.print(',');
    file.print(counts[10]);
    file.print(',');
    file.print(counts[11]);
    file.print(',');
    file.print("RAW VALUES");
    file.print(',');
    file.print(readings[0]);
    file.print(',');
    file.print(readings[1]);
    file.print(',');
    file.print(readings[2]);
    file.print(',');
    file.print(readings[3]);
    file.print(',');
    file.print(readings[6]);
    file.print(',');
    file.print(readings[7]);
    file.print(',');
    file.print(readings[8]);
    file.print(',');
    file.print(readings[9]);
    file.print(',');
    file.print(readings[10]);
    file.print(',');
    file.print(readings[11]);
    file.print(',');
    file.println(measurementType);
    file.close();
  } else {
    Serial.println(F("Error opening file!"));
  }
}

void serialprintresults(RTC_PCF8523 &rtc, const char* measurementType, uint16_t readings[]) {
  if (strcmp(measurementType, "Chlorophyll-a") == 0) {
    Serial.print("Raw F8: ");
    Serial.println(readings[9]);
    sumChl_F8 += readings[9];
    countChl_F8++;
  } else if (strcmp(measurementType, "Turbidity") == 0) {
    Serial.print("Raw NIR: ");
    Serial.println(readings[11]);
    sumTurb_NIR += readings[11];
    countTurb_NIR++;
  } else if (strcmp(measurementType, "Background") == 0) {
    Serial.print("Raw F8: ");
    Serial.println(readings[9]);
    sumChl_BG += readings[9];
    countChl_BG++;
    Serial.print("Raw NIR: ");
    Serial.println(readings[11]);
    sumTurb_BG += readings[11];
    countTurb_BG++;
  }
}
