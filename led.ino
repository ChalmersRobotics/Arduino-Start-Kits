#include <FastLED.h> // https://github.com/FastLED/FastLED
#include <Button.h> // https://github.com/madleech/Button

// Define pins for button and led
#define MODE_BUTTON_PIN 4
#define LED_PIN 5

// The number of LEDs in the serie
#define NUM_LEDS 2

// Holds the LED data
CRGB leds[NUM_LEDS];

// Simple button
Button modeButton(MODE_BUTTON_PIN); 

// The current mode
uint8_t mode = 0;
// If the button is currently pressed or not
boolean pressed = false;

// Setup code, will be called once on powerup
void setup() {
  // Initiate FastLED library, with APA106 as LED type
  FastLED.addLeds<APA106, LED_PIN>(leds, NUM_LEDS);
  modeButton.begin();
}

// This method will be called repeatedly
void loop() {

  // Check if button has been pressed, but only run once when pressed
  // This section cycles through the modes
  if (modeButton.pressed() && !pressed) {
    pressed = true;
    mode++;

    if (mode >= 4) {
      mode = 0;
    }
  }

  if (modeButton.released()) {
    pressed = false;
  }

  // Depending on mode we will run different effects
  if (mode == 0) {
    rainbow();
  } else if (mode == 1) {
    blink();
  } else if (mode == 2) {
    randomBlink();
  } else if (mode == 3) {
    randomFade();
  }
}

// Simple rainbow fade
void rainbow() {
  // Cycle through all HSV hues
  for (int hue = 0; hue < 255; hue++) {
    // Both LEDs the same hue
    leds[0] = CHSV(hue, 255, 255);
    leds[1] = CHSV(hue, 255, 255);

    FastLED.show();

    // The amount of milliseconds to wait until next increment of hue, while also keep tracking of button press
    // so that we don't have to wait for whole loop cycle before being detected
    delayButton(30);

    // Break the loop after we have released the button
    if (pressed && modeButton.released()) {
      pressed = false;
      leds[0] = CHSV(255,255,0);

      break;
    }
  }
}

// Blink red and blue, alternate on the 2 LEDs
void blink() {
  leds[0] = CHSV(0, 255, 255);
  leds[1] = CHSV(168, 255, 255);
  FastLED.show();

  delayButton(500);

  leds[1] = CHSV(0, 255, 255);
  leds[0] = CHSV(168, 255, 255);
  FastLED.show();

  delayButton(500);
}

// Blink random clolors
void randomBlink() {
  leds[0] = CHSV(random(0, 255), 255, 255);
  leds[1] = CHSV(random(0, 255), 255, 255);
  FastLED.show();
  delayButton(200);
}

int oldHue = 0;
// Fade to random hues
void randomFade() {
  int newHue = oldHue;
  int way = random(0, 1);
  for (int hue = 0; hue < random(1, 50); hue++) {

    if (way == 0) {
      newHue--;
    } else {
      newHue++;      
    }

    leds[0] = CHSV(newHue, 255, 255);
    leds[1] = CHSV(newHue, 255, 255);

    FastLED.show();
    delayButton(10);

    if (pressed && modeButton.released()) {
      pressed = false;
      leds[0] = CHSV(255,255,0);

      break;
    }
  }

  oldHue = newHue;
  delayButton(100);
}

void delayButton(int delayTime) {
  for (int i = 0; i < delayTime; i++) {
    delay(1);
    if (modeButton.pressed() && !pressed) {
        pressed = true;
        mode++;

        if (mode >= 4) {
          mode = 0;
        }
    }
  }
}