#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin not used for most OLED modules

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Initialize OLED display with I2C address 0x3C (for the 128x64)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  // Clear the display buffer
  display.clearDisplay();
  display.display();
  
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming byte
    String newText = Serial.readString();
    // Clear the display buffer
    display.clearDisplay();
    // Set text size, color, and position
    display.setTextSize(1); // Adjust the font size here
    display.setTextColor(SSD1306_WHITE);
    // Print the entire text on the screen
    display.setCursor(0, 0);
    display.println(newText);
    // Display the text on the OLED screen
    display.display();
  }
}
