#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <FastLED.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 // adapte selon ton écran
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

  // Initialisation OLED
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

  // Initialisation BME280
  if (!bme.begin(0x76)) {
    Serial.println("BME280 non détecté !");
    while (1);
  }

  // Initialisation LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // Lecture capteur
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  // Affichage OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.print("Hum: ");
  display.print(humidity);
  display.println(" %");

  display.print("Pres: ");
  display.print(pressure);
  display.println(" hPa");

  display.display();

  // Affichage LEDs
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  // Température (rouge) LEDs 0-2
  int tempLeds = map(constrain(temperature, 0, 40), 0, 40, 0, 3);
  for (int i = 0; i < tempLeds; i++) {
    leds[i] = CRGB::Red;
  }

  // Humidité (bleu) LEDs 3-5
  int humLeds = map(constrain(humidity, 0, 100), 0, 100, 0, 3);
  for (int i = 3; i < 3 + humLeds; i++) {
    leds[i] = CRGB::Blue;
  }

  // Pression (vert) LEDs 6-7
  int presLeds = map(constrain(pressure, 950, 1050), 950, 1050, 0, 2);
  for (int i = 6; i < 6 + presLeds; i++) {
    leds[i] = CRGB::Green;
  }

  FastLED.show();
  delay(2000);
}
