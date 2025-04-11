#define LED_PIN 4
#define BUTTON 10


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON, INPUT);

}


void loop() {
  // put your main code here, to run repeatedly:

  while (digitalRead(BUTTON) == LOW) {
    digitalWrite(LED_PIN, LOW);
    delay(300);
    digitalWrite(LED_PIN, HIGH);
    delay(300);
  }

}
