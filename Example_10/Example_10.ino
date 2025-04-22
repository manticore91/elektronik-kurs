
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSans9pt7b.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SDA_PIN 8
#define SCL_PIN 9 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {

  Wire.begin(SDA_PIN, SCL_PIN);

  if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    display.clearDisplay();
    display.display();
  }

}

void loop(){

  display.clearDisplay();

  display.setFont(&FreeSans9pt7b); // ≈ Schriftgröße 1.5
  display.setTextSize(1);          // notwendig bei GFX Fonts
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 15);
  display.print("Hallo ");
  
    
  display.setCursor(0, 32);
  display.print("Welt!");
  
  display.display();
}
