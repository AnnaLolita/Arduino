#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <MPU6050.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDRESS 0x3C

// I2C ESP32
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MPU6050 mpu;

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
  display.println("MPU-6050 Test");
  display.display();
  delay(2000);

  // Initialisation MPU6050
  Serial.println("Initialisation MPU6050...");
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connecté !");
  } else {
    Serial.println("Erreur de connexion MPU6050 !");
    while (1);
  }
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Conversion en g (optionnel)
  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;

  // Affichage série
  Serial.print("Acc X: "); Serial.print(axg);
  Serial.print(" Y: "); Serial.print(ayg);
  Serial.print(" Z: "); Serial.println(azg);

  // Affichage OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("X: "); display.print(axg, 2);
  display.setCursor(0, 10);
  display.print("Y: "); display.print(ayg, 2);
  display.setCursor(0, 20);
  display.print("Z: "); display.print(azg, 2);
  display.display();

  delay(500);
}
