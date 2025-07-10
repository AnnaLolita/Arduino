#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDRESS 0x3C

// HC-SR04
#define TRIG_PIN 18
#define ECHO_PIN 19

// Objets
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Initialisation OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("Écran OLED non détecté !");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("HC-SR04 Test");
  display.display();
  delay(2000);

  // Initialisation pins HC-SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Déclenchement du capteur
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Lecture du temps echo
  long duration = pulseIn(ECHO_PIN, HIGH);
  // Calcul distance en cm
  float distance = duration * 0.034 / 2;

  // Affichage série
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Affichage OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Distance:");
  display.setCursor(0, 10);
  display.print(distance);
  display.println(" cm");
  display.display();

  delay(500);
}
