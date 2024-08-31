#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "NodeMCU";
const char* password = "12345678";
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

ESP8266WebServer server(80);
WiFiClient client;

String receivedText = "";

// Local IP address for the soft access point
IPAddress localIP(192, 168, 1, 1);

void setup() {
  Serial.begin(9600);
  delay(100);

  // Set up OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Use 0x3C as the I2C address
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  // Connect to WiFi
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(localIP, localIP, IPAddress(255, 255, 255, 0)); // Set local IP, gateway, and subnet
  delay(100);

  // Print IP address
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(myIP);

  // Start web server
  server.on("/", handleRoot);
  server.on("/submit", HTTP_POST, handleSubmit);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>body { font-family: Arial, sans-serif; margin: 50px; }";
  html += "h1 { text-align: center; } form { max-width: 300px; margin: 0 auto; }";
  html += "input[type=text], input[type=submit] { width: 100%; padding: 10px; margin: 5px 0; }";
  html += "input[type=submit] { background-color: #4CAF50; color: white; border: none; cursor: pointer; }";
  html += "input[type=submit]:hover { background-color: #45a049; }</style>";
  html += "<body><h1>Translation Service</h1>";
  html += "<form method='post' action='/submit'>";
  html += "<input type='text' name='text' placeholder='Enter text to translate'>";
  html += "<input type='submit' value='Translate'>";
  html += "</form>";

  if (receivedText.length() > 0) {
    html += "<h2>Translated Text:</h2>";
    html += "<p>" + receivedText + "</p>";
  }

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleSubmit() {
  receivedText = server.arg("text");
  Serial.println("Received text: " + receivedText);
  
  // Display received text on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Received text:");
  display.println(receivedText);
  display.display();
  
  // Redirect back to the form after 3 seconds
  server.sendHeader("Location", "/");
  server.send(302);
}
