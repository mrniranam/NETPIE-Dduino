/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 */
#include "ESP8266WiFi.h"
#include "Adafruit_SSD1306.h"
#include "Wire.h"

#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_RESET);
int line = 0;

void setup() {
  Serial.begin(115200);
  Wire.pins(D1, D2);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.setTextSize(1);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  Serial.println("scan start");
  display.clearDisplay();
  display.setCursor(0, 5);
  line = 0;

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      
      display.print(i + 1);
      display.print(": ");
      if(WiFi.SSID(i).length()<=12){
        display.print(WiFi.SSID(i));
      }else{
        //shorten wifi name, so it fits the screen
        display.print(WiFi.SSID(i).substring(0, 6));
        display.print("...");
        display.print(WiFi.SSID(i).substring(WiFi.SSID(i).length()-5));
      }
      display.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      display.display();
      if(line < 6){
        line = line + 1;
      }else{
        line = 0;
        display.setCursor(0, 5);
        display.clearDisplay();
        delay(2000);
      }
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}