#include <Adafruit_NeoPixel.h>

#define NEO_PIN    1 
#define NUM_PIXELS 10 
#define DELAY_VALUE 500

Adafruit_NeoPixel pixels(NUM_PIXELS, NEO_PIN, NEO_RGB + NEO_KHZ800);


void setup() {

  pixels.begin();

}

void loop() {
  pixels.clear();

    for(int i = 0; i < NUM_PIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();

    delay(DELAY_VALUE);
  }
}
