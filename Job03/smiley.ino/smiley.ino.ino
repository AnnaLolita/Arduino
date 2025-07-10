#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Wire.begin(21, 22);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();

  // Visage (cercle plus petit adapté à la hauteur)
  display.drawCircle(64, 16, 10, SSD1306_WHITE);

  // Œil gauche
  display.fillCircle(60, 12, 1, SSD1306_WHITE);

  // Œil droit
  display.fillCircle(68, 12, 1, SSD1306_WHITE);

  // Bouche (ligne courte)
  display.drawLine(61, 20, 67, 20, SSD1306_WHITE);

  display.display();
}

void loop() {
  // Rien ici
}
