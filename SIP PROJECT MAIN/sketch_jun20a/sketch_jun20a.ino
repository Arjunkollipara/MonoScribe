#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "NodeMCU";
const char* password = "123456789";
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ESP8266WebServer server(80);
WiFiClient client;

String receivedText = "";

IPAddress localIP(192, 168, 1, 1);

struct Translation {
  String inputText;
  String translatedText;
};


Translation translations[] = {
  // Chinese to English
  {"你好", "hello"},
  {"再见", "goodbye"},
  {"谢谢", "thank you"},
  {"是的", "yes"},
  {"不", "no"},
  {"狗", "dog"},
  {"猫", "cat"},
  {"水", "water"},
  {"食物", "food"},
  {"学校", "school"},
  {"手机", "phone"},
  {"电脑", "computer"},
  {"家庭", "family"},
  {"朋友", "friend"},
  {"工作", "work"},
  {"旅行", "travel"},
  {"时间", "time"},
  {"天气", "weather"},
  {"健康", "health"},
  {"快乐", "happy"},
  {"悲伤", "sad"},
  {"翻译", "translation"},
  {"成果", "success"},
  {"失败", "failure"},
  {"电视", "television"},
  {"音乐", "music"},
  {"电影", "movie"},
  {"书籍", "book"},
  {"艺术", "art"},
  {"运动", "sports"},
  {"游戏", "game"},
  {"社交", "social"},
  {"星期", "week"},
  {"月份", "month"},
  {"年份", "year"},
  
  // Japanese to English
  {"こんにちは", "hello"},
  {"さようなら", "goodbye"},
  {"ありがとう", "thank you"},
  {"はい", "yes"},
  {"いいえ", "no"},
  {"犬", "dog"},
  {"猫", "cat"},
  {"水", "water"},
  {"食べ物", "food"},
  {"学校", "school"},
  {"携帯電話", "cell phone"},
  {"コンピュータ", "computer"},
  {"家族", "family"},
  {"友達", "friend"},
  {"仕事", "work"},
  {"旅行", "travel"},
  {"時間", "time"},
  {"天気", "weather"},
  {"健康", "health"},
  {"幸せ", "happy"},
  {"悲しい", "sad"},
  {"翻訳", "translation"},
  {"成功", "success"},
  {"失敗", "failure"},
  {"テレビ", "television"},
  {"音楽", "music"},
  {"映画", "movie"},
  {"本", "book"},
  {"アート", "art"},
  {"スポーツ", "sports"},
  {"ゲーム", "game"},
  {"社会", "social"},
  {"週", "week"},
  {"月", "month"},
  {"年", "year"},
  
  // Russian to English
  {"привет", "hello"},
  {"до свидания", "goodbye"},
  {"спасибо", "thank you"},
  {"да", "yes"},
  {"нет", "no"},
  {"собака", "dog"},
  {"кошка", "cat"},
  {"вода", "water"},
  {"еда", "food"},
  {"школа", "school"},
  {"телефон", "phone"},
  {"компьютер", "computer"},
  {"семья", "family"},
  {"друг", "friend"},
  {"работа", "work"},
  {"путешествие", "travel"},
  {"время", "time"},
  {"погода", "weather"},
  {"здоровье", "health"},
  {"счастливый", "happy"},
  {"печальный", "sad"},
  {"перевод", "translation"},
  {"успех", "success"},
  {"провал", "failure"},
  {"телевизор", "television"},
  {"музыка", "music"},
  {"фильм", "movie"},
  {"книга", "book"},
  {"искусство", "art"},
  {"спорт", "sports"},
  {"игра", "game"},
  {"социальный", "social"},
  {"неделя", "week"},
  {"месяц", "month"},
  {"год", "year"}
};

const int numTranslations = sizeof(translations) / sizeof(translations[0]);

void setup() {
  Serial.begin(9600);
  delay(100);

  
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  // Connect to WiFi
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(localIP, localIP, IPAddress(255, 255, 255, 0));
  delay(100);

  // Print IP address
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(myIP);

  // Start web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/submit", HTTP_POST, handleSubmit);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Translation Service</title>";
  html += "<style>body { font-family: Arial, sans-serif; margin: 50px; background-color: #f0f0f0; }";
  html += "h1 { text-align: center; } form { max-width: 300px; margin: 0 auto; background-color: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }";
  html += "input[type=text], input[type=submit] { width: 100%; padding: 10px; margin: 10px 0; border: 1px solid #ccc; border-radius: 4px; }";
  html += "input[type=submit] { background-color: #4CAF50; color: white; border: none; cursor: pointer; }";
  html += "input[type=submit]:hover { background-color: #45a049; }";
  html += "p { margin-top: 20px; text-align: center; }</style>";
  html += "<script>";
  html += "function handleSubmit() {";
  html += "  var text = document.getElementById('text').value;";
  html += "  if (text.trim() !== '') {";
  html += "    document.getElementById('translate-form').submit();";
  html += "  } else {";
  html += "    alert('Please enter text to translate.');";
  html += "  }";
  html += "}";
  html += "</script>";
  html += "</head>";
  html += "<body>";
  html += "<h1>Translation Service</h1>";
  html += "<form id='translate-form' method='post' action='/submit'>";
  html += "<input type='text' id='text' name='text' placeholder='Enter text to translate' required>";
  html += "<br><br>";
  html += "<input type='button' value='Translate' onclick='handleSubmit()'>";
  html += "</form>";

  if (receivedText.length() > 0) {
    html += "<p><strong>Translated Text:</strong></p>";
    html += "<p>" + translateSentence(receivedText) + "</p>";  
  }

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleSubmit() {
  receivedText = server.arg("text");
  Serial.println("Received text: " + receivedText);

  
  String translatedText = translateSentence(receivedText);

 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int startIndex = 0;
  int endIndex = translatedText.indexOf(' ');

  while (endIndex != -1) {
    String word = translatedText.substring(startIndex, endIndex);
    displayWordCentered(word);
    delay(1000);
    startIndex = endIndex + 1;
    endIndex = translatedText.indexOf(' ', startIndex);
  }

  
  String word = translatedText.substring(startIndex);
  displayWordCentered(word);
  delay(1000);


  server.sendHeader("Location", "/");
  server.send(302);
}

void displayWordCentered(String word) {
  int16_t x1, y1;
  uint16_t w, h;
  display.clearDisplay();
  display.getTextBounds(word, 0, 0, &x1, &y1, &w, &h);
  int16_t x = (SCREEN_WIDTH - w) / 2;
  int16_t y = (SCREEN_HEIGHT - h) / 2;
  display.setCursor(x, y);
  display.println(word);
  display.display();
}

String translateSentence(String inputText) {
  String translatedSentence = "";
  int startIndex = 0;
  int endIndex = inputText.indexOf(' ');

  while (endIndex != -1) {
    String word = inputText.substring(startIndex, endIndex);
    translatedSentence += translateWord(word) + " ";
    startIndex = endIndex + 1;
    endIndex = inputText.indexOf(' ', startIndex);
  }

  translatedSentence += translateWord(inputText.substring(startIndex));
  return translatedSentence;
}\

String translateWord(String inputText) {
  for (int i = 0; i < numTranslations; ++i) {  
    if (inputText.equals(translations[i].inputText)) {
      return translations[i].translatedText; 
    }
  }
  return inputText; 
}