#include <Adafruit_NeoPixel.h>

#define N_PIXELS 30  // number of pixels in the strip
#define MIC_PIN 1
#define LED_PIN 0
#define SAMPLE_WINDOW 50
#define THRESHOLD 500 // minimum level required to change colors
#define IDLE_TIME 5000 // how long to wait before turning off the LEDs

unsigned int sample;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned int colorIndex = 0;
uint32_t colors[] = {
  strip.Color(255, 0, 0),
  strip.Color(0, 255, 0),
  strip.Color(0, 0, 255),
  strip.Color(255, 255, 255)
};

// keeps track of how long it has been since the color changed
unsigned long lastChange = 0;

void setup() {
  strip.begin();
}

void loop() {
  
  unsigned int level = getLevel();

  // changes color if the signal is strong enough
  if (level > THRESHOLD) {
    
    setStrip(colors[colorIndex]);
  
    lastChange = millis();
  
    // go to the next color
    colorIndex++;
    if (colorIndex >= (sizeof(colors) / sizeof(uint32_t))) {
      colorIndex = 0;
    }
  }

  // turns off the lights if there hasn't been a sound for a while
  if (since(lastChange) > (5000)) {
    setStrip(strip.Color(0, 0, 0));
  }
}

// returns a value between 0 and 1023 representing the sound level
// from https://learn.adafruit.com/adafruit-microphone-amplifier-breakout/measuring-sound-levels
unsigned int getLevel() {
  unsigned long sampleStart = millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (since(sampleStart) < SAMPLE_WINDOW)
  {
    sample = analogRead(MIC_PIN);
    if (sample < 1024)  // toss out spurious readings
    {
      signalMax = max(sample, signalMax);  // save just the max levels
      signalMin = min(sample, signalMin);  // save just the min levels
    }
  }

  return signalMax - signalMin;
}

// returns the number of miliseconds since the given timestamp
unsigned int since(unsigned int t) {
  return millis() - t;
}

// sets all the LEDs in the strip to the given color
void setStrip(uint32_t color) {
  for(int i=0; i < N_PIXELS; i++) {  
    strip.setPixelColor(i, color);
  }
  strip.show();
}

