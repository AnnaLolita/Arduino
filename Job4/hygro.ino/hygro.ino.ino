#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <FastLED.h>

// Définition des pins et paramètres I2C
#define SDA_PIN 21
#define SCL_PIN 22

// Définition des paramètres WS2812
#define LED_PIN 5
#define NUM_LEDS 8
#define BRIGHTNESS 50
#define LED_TYPE WS2812
#define COLOR_ORDER GRB

// Objets capteur et LEDs
Adafruit_BME280 bme;
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialisation BME280
  if (!bme.begin(0x76)) { // 0x76 si SDO à GND, 0x77 si SDO à VCC
    Serial.println("BME280 non détecté, vérifie les branchements !");
    while (1);
  }
  Serial.println("BME280 détecté !");

  // Initialisation LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // Lecture des données du capteur
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  Serial.print("Température = "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Humidité = "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Pression = "); Serial.print(pressure); Serial.println(" hPa");
  Serial.println("---------------------");

  // Nettoyer le bandeau avant affichage
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  // Bargraph température (rouge) sur LEDs 0-2
  int tempLeds = map(constrain(temperature, 0, 40), 0, 40, 0, 3);
  for (int i = 0; i < tempLeds; i++) {
    leds[i] = CRGB::Red;
  }

  // Bargraph humidité (bleu) sur LEDs 3-5
  int humLeds = map(constrain(humidity, 0, 100), 0, 100, 0, 3);
  for (int i = 3; i < 3 + humLeds; i++) {
    leds[i] = CRGB::Blue;
  }

  // Bargraph pression (vert) sur LEDs 6-7
  int presLeds = map(constrain(pressure, 950, 1050), 950, 1050, 0, 2);
  for (int i = 6; i < 6 + presLeds; i++) {
    leds[i] = CRGB::Green;
  }

  // Mise à jour des LEDs
  FastLED.show();

  delay(2000); // rafraîchissement toutes les 2s
}
