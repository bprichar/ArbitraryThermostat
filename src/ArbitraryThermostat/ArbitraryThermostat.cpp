#include "DHT.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"

#include <Wire.h>
#include "RTClib.h"

// The Clock
RTC_DS1307 rtc;

// Temperature Sensor Stuff
#define DHTPIN 2     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

// These are the four touchscreen pins
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 8   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 110
#define TS_MINY 80
#define TS_MAXX 900
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// The display uses hardware SPI, plus #9 & #10
#define TFT_RST -1  // dont use a reset pin, tie to arduino RST if you like
#define TFT_DC 9
#define TFT_CS 10

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setup() {
  Wire.begin();
  rtc.begin();
  dht.begin();
  tft.begin(HX8357D);
  tft.fillScreen(HX8357_BLACK);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0, 10);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(3);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    tft.println("Failed to read from DHT sensor!");
  }
  else {
    tft.println("Humidity: ");
    tft.println(h);
    tft.println("Temperature: ");
    tft.println(f);
  }

  tft.setCursor(0, 200);
  if (! rtc.isrunning()) {
    tft.println("RTC is NOT running!");
  }
  else {
    DateTime now = rtc.now();
    tft.println(now.hour());
    tft.println(now.minute());
    tft.println(now.second());
   } 
}
