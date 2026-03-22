#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

#include <esp_sleep.h>

#include <Wire.h>

#include "SparkFunBME280.h"

#include "config.h"
#include "types.h"

// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  5000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 6*LOG_INTERVAL // mills between calls to flush() - to write data to the card
//uint32_t syncTime = 0; // time of last sync()

#define ECHO_TO_SERIAL   1 // echo data to serial port
//#define WAIT_TO_START    0 // Wait for serial input in setup()

// the digital pins that connect to the LEDs
#define redLEDpin 25
#define greenLEDpin 4

#define ONE_WIRE_BUS 15  // DS18B20 pin

#define USER_PWD_LEN 40

#define OLED_RESET 4

// Define CS pin for the SD card module
#define SD_CS 5

#define ALTITUDE 600.0

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;


BME280 bme280; //Global sensor object

RTC_DATA_ATTR int bootCount = 0;
// Define deep sleep options
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// Sleep for 10 minutes = 600 seconds
uint64_t TIME_TO_SLEEP = 60;


const uint64_t updateInterval = 1000 * 3600 * 1; // 1 hour
uint64_t lastUpdate = 0;

// Save reading number on RTC memory
RTC_DATA_ATTR int readingID = 0;

String dataMessage;

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

unsigned long timenow = 0;
unsigned long duration = 0;


//char unameenc[USER_PWD_LEN];
float oldTemp_a;
float oldTemp_b;
