

#define LED_PIN 13
#define BUTTON_PIN 22

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

//void loop() {
//  int buttonState = digitalRead(BUTTON_PIN);

//  if (buttonState == LOW) {
//    digitalWrite(LED_PIN, HIGH); // Bouton appuyé -> LED ON
//  } else {
//   digitalWrite(LED_PIN, LOW); // Bouton relâché -> LED OFF
// }
//}

void loop() {
  static int pressCount = 0;
  static int lastButtonState = HIGH;
  static unsigned long lastPressTime = 0;

  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) { // détection front descendant = nouvel appui
    pressCount++;
    lastPressTime = millis();
    Serial.print("Appui detecte. Compteur = ");
    Serial.println(pressCount);
  }

  lastButtonState = buttonState;

  // Si plus de 1 seconde s'est écoulée depuis le dernier appui, agir selon le nombre d'appuis
  if ((millis() - lastPressTime) > 1000 && pressCount > 0) {
    Serial.print("Action pour ");
    Serial.print(pressCount);
    Serial.println(" appui(s)");

    if (pressCount == 1) {
      digitalWrite(LED_PIN, HIGH); pressCount = 0;// 1 appui -> LED ON
    } else if (pressCount == 2) {
      digitalWrite(LED_PIN, HIGH); pressCount = 0; // 2 appuis -> LED reste ON (identique pour l’instant)
    } else if (pressCount == 3) {
      digitalWrite(LED_PIN, LOW);pressCount = 0; // 3 appuis -> LED OFF (clignotement à ajouter plus tard)
    }
    //pressCount = 0; // reset du compteur après action
  }
}
