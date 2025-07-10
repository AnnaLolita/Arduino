#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <FastLED.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDRESS 0x3C

// I2C ESP32
#define SDA_PIN 21
#define SCL_PIN 22

// WS2812
#define LED_PIN 5
#define NUM_LEDS 8
#define BRIGHTNESS 50
#define LED_TYPE WS2812
#define COLOR_ORDER GRB

// Objets capteurs et LEDs
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_BME280 bme;
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("Écran OLED non détecté !");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Station Meteo");
  display.display();
  delay(2000);

  if (!bme.begin(0x76)) {
    Serial.println("BME280 non détecté !");
    while (1);
  }

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("T:");
  display.print(temperature);
  display.print("C");

  display.setCursor(64, 0);
  display.print("H:");
  display.print(humidity);
  display.print("%");

  display.setCursor(0, 8);
  display.print("P:");
  display.print(pressure);
  display.println("hPa");

  // Bargraph Température
  int tempBar = map(constrain(temperature, 0, 40), 0, 40, 0, SCREEN_WIDTH);
  display.fillRect(0, 16, tempBar, 4, SSD1306_WHITE);

  // Bargraph Humidité
  int humBar = map(constrain(humidity, 0, 100), 0, 100, 0, SCREEN_WIDTH);
  display.fillRect(0, 22, humBar, 4, SSD1306_WHITE);

  // Bargraph Pression
  int presBar = map(constrain(pressure, 950, 1050), 950, 1050, 0, SCREEN_WIDTH);
  display.fillRect(0, 28, presBar, 4, SSD1306_WHITE);

  display.display();

  // LEDs
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  int tempLeds = map(constrain(temperature, 0, 40), 0, 40, 0, 3);
  for (int i = 0; i < tempLeds; i++) {
    leds[i] = CRGB::Red;
  }

  int humLeds = map(constrain(humidity, 0, 100), 0, 100, 0, 3);
  for (int i = 3; i < 3 + humLeds; i++) {
    leds[i] = CRGB::Blue;
  }

  int presLeds = map(constrain(pressure, 950, 1050), 950, 1050, 0, 2);
  for (int i = 6; i < 6 + presLeds; i++) {
    leds[i] = CRGB::Green;
  }

  FastLED.show();

  delay(2000);
}
