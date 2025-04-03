/*
  Nologo esp32c3 supermini
  USBMODE HARDWARE CDC AND JTAG
  USB CDC ON BOOT Enabled

  Wenn das OLED Display nicht gefunden werden konnte, blinkt die LED 13!*

  *diese Funktion konnte ich leider nicht umsetzten,
  da die initialation sogar mit Abgelöteten OLED Display ein ,,gefunden!" zurückgibt!

  E (1328) i2c.master: s_i2c_synchronous_transaction(918): I2C transaction failed
  E (1335) i2c.master: i2c_master_multi_buffer_transmit(1180): I2C transaction failed
  OLED gefunden!
  E (1343) i2c.master: I2C transaction unexpected nack detected
  E (1348) i2c.master: s_i2c_synchronous_transaction(918): I2C transaction failed

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED nicht gefunden!");
    OLEDInit = 0;
  } else {
    Serial.println("OLED gefunden!");
    OLEDInit = 1;
    display.clearDisplay();
    display.display();
  }

  daher habe ich diese Funktion nicht implementiert.

  Die Onboardled ist mit GPIO 8 verbunden, der ist gleichzeitig i2c deswegen Leuchtet sie.
  zum glück keine interferenzen.
*/

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>
#include <Fonts/FreeSans9pt7b.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SDA_PIN 8
#define SCL_PIN 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- DHT11 ----------
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------- WS2812 ----------
#define LED_PIN 1
#define NUM_LEDS 10
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ---------- GPIO ----------
#define POTI_PIN 0
#define BUTTON_PIN 10
#define led13 4  // active LOW

// ---------- Variablen ----------
uint16_t rainbowPos = 0;
unsigned long lastSerialTime = 0;
unsigned long lastFadeTime = 0;

// ---------- LED folgt Button direkt ----------
void handleLEDFromButton() {
  int buttonState = digitalRead(BUTTON_PIN);
  digitalWrite(led13, buttonState); // LOW = AN, HIGH = AUS (active LOW)
}

// ---------- OLED-Anzeige mit FreeSans7pt7b ----------
void updateOLED() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int poti = analogRead(POTI_PIN);
  int buttonState = digitalRead(BUTTON_PIN);
  bool ledState = (buttonState == LOW); // active LOW = LED an bei LOW

  display.clearDisplay();

  display.setFont(&FreeSans9pt7b); // ≈ Schriftgröße 1.5
  display.setTextSize(1);          // notwendig bei GFX Fonts
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 15);
  display.print("TEMP: ");
  if (isnan(temp)) {
    display.print("404");
  } else {
    display.print((int)temp);
    display.print("C");
  }

  display.setCursor(0, 32);
  display.print("HUM: ");
  if (isnan(hum)) {
    display.print("404");
  } else {
    display.print((int)hum);
    display.print("%");
  }

  display.setCursor(0, 49);   // Dritte Zeile
  display.print("POTI: "); display.println(poti);

  display.setCursor(0, 64);   // Vierte Zeile
  display.print("BTN: "); display.print(buttonState == HIGH ? 1 : 0);
  display.print(" - LED: "); display.println(!ledState);

  display.display();
}

// ---------- Serielle Ausgabe alle 1 Sekunde ----------
void printSerialDataEverySecond() {
  if (millis() - lastSerialTime >= 1000) {
    lastSerialTime = millis();

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int poti = analogRead(POTI_PIN);
    int buttonState = digitalRead(BUTTON_PIN);

    Serial.println("---- Werte ----");
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println(" C");
    Serial.print("Hum:  ");
    Serial.print(hum);
    Serial.println(" %");
    Serial.print("Poti: ");
    Serial.println(poti);
    Serial.print("Button: ");
    Serial.println(buttonState);
    Serial.print("LED13: ");
    Serial.println(buttonState);
    Serial.println();
  }
}

// ---------- Rainbow-Fade für WS2812 ----------
void rainbowFadeSmooth() {
  if (millis() - lastFadeTime >= 1) {
    lastFadeTime = millis();
    for (int i = 0; i < NUM_LEDS; i++) {
      int pixelIndex = (rainbowPos + i * 256 / NUM_LEDS) & 255;
      strip.setPixelColor(i, Wheel(pixelIndex));
    }
    strip.show();
    rainbowPos = (rainbowPos + 1) & 255;
  }
}

// ---------- Farbverlauf ----------
uint32_t Wheel(byte pos) {
  pos = 255 - pos;
  if (pos < 85) return strip.Color(255 - pos * 3, 0, pos * 3);
  if (pos < 170) {
    pos -= 85;
    return strip.Color(0, pos * 3, 255 - pos * 3);
  }
  pos -= 170;
  return strip.Color(pos * 3, 255 - pos * 3, 0);
}

// ---------- Fehler-LED blinken ----------
void blinkErrorLED() {
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  if (millis() - lastBlink >= 100) {
    lastBlink = millis();
    ledState = !ledState;
    digitalWrite(led13, ledState ? LOW : HIGH); // active LOW
  }
}

void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println("=== Ralph's Workshop PCB Test Programm ===");
  delay(50);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(led13, OUTPUT);
  digitalWrite(led13, HIGH); // LED AUS (active LOW)

  dht.begin();
  strip.begin();
  strip.setBrightness(128);
  strip.show();

  Wire.begin(SDA_PIN, SCL_PIN);

  if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    display.clearDisplay();
    display.display();
  }

}

void loop() {
  handleLEDFromButton();
  updateOLED();
  rainbowFadeSmooth();
  printSerialDataEverySecond();
}
