#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 30
Adafruit_SSD1306 display(OLED_RESET);
void setup() {
  Wire.pins(D1, D2);
  float temp = 0;
  // read values from actual temp
display.begin(SSD1306_SWITCHCAPVCC, 0x3c); //initialize I2C addr 0x3c
display.clearDisplay(); // clears the screen and buffer
display.drawPixel(127, 63, WHITE);

display.drawLine(0, 63, 127, 21, WHITE);
display.drawCircle(110, 50, 12, WHITE);
display.fillCircle(45, 50, 8, WHITE);
display.drawTriangle(70, 60, 90, 60, 80, 46, WHITE);
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("Welcome All to");
display.setTextSize(2);
display.println(temp);
//display.println("\^-^/");
display.setTextColor(BLACK, WHITE);
display.setTextSize(1);
display.println("www.ArduinoAll.com");
display.setTextColor(WHITE, BLACK);
display.display();
}

void loop() {
}
