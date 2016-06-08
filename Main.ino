/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MicroGear.h>

const char* ssid     = "ssid";
const char* password = "password";

#define APPID   "APPID"
#define KEY     "KEY"
#define SECRET  "SECRET"
#define ALIAS   "d-duino"
#define DHTPIN  0
#define DHTTYPE DHT22
#define OLED_RESET 30

Adafruit_SSD1306 display(OLED_RESET);

WiFiClient client;
AuthClient *authclient;

int timer = 0;
MicroGear microgear(client);
DHT dht(DHTPIN, DHTTYPE);

static const unsigned char PROGMEM gear[] =
{ 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xC0,
0x00, 0x00, 0x00, 0x3F, 0xFF, 0xE0, 0x3E, 0x00, 0x00, 0x3F, 0xFF, 0xF8, 0xFF, 0x00, 0x00, 0x0F,
0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x0F, 0xE0, 0x0F, 0xF0, 0x00, 0x00, 0x1F, 0x80, 0x01, 0x80, 0x38,
0x3F, 0xFE, 0x00, 0x00, 0x01, 0xFC, 0x7F, 0xFC, 0x07, 0xE0, 0x07, 0xFC, 0x7F, 0xFC, 0x1F, 0xF8,
0x3F, 0xC0, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x80, 0x00, 0x00,
0x1F, 0xF0, 0x7F, 0x80, 0x3F, 0xFC, 0x00, 0x00, 0x1F, 0xE0, 0x3F, 0xFF, 0x00, 0x00, 0x03, 0xFC,
0x00, 0x3F, 0xC0, 0x03, 0x00, 0x78, 0x00, 0x0F, 0xFC, 0x3F, 0xC0, 0x00, 0x00, 0x1F, 0xFF, 0xFF,
0xF8, 0x00, 0x00, 0x3F, 0xFF, 0xF8, 0xFF, 0x00, 0x00, 0x3F, 0xFF, 0xE0, 0x3E, 0x00, 0x00, 0x0F,
0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM waterDrop[] =
{ B00000000, B00000000,
  B00000001, B10000000,
  B00000001, B10000000,
  B00000011, B11000000,
  B00001111, B11110000,
  B00111111, B11101100,
  B01111111, B11100110,
  B01111111, B11100110,
  B00111111, B11001100,
  B00001111, B11110000 };

static const unsigned char PROGMEM thermo[] =
{ 
  B00000011, B11000000, B00000110, B01100000, B00000111, B11100000, B00000110, B01100000, B00000111, 
B11100000, B00000110, B01100000, B00000110, B01100000, B00001100, B00110000, B00011111, B11111000, 
B00001111, B11110000, B00000011, B11000000 };

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setName(ALIAS);
}

void setup() {
    /* Add Event listeners */

    /* Call onMsghandler() when new message arraives */
    microgear.on(MESSAGE,onMsghandler);

    /* Call onFoundgear() when new gear appear */
    microgear.on(PRESENT,onFoundgear);

    /* Call onLostgear() when some gear goes offline */
    microgear.on(ABSENT,onLostgear);

    /* Call onConnected() when NETPIE connection is established */
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
    dht.begin();
    Wire.pins(D1, D2);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3c); //initialize I2C addr 0x3c
    display.clearDisplay(); // clears the screen and buffer

    display.setTextColor(WHITE);
    display.setCursor(50, 5);
    display.setTextSize(1);
    display.print("Loading...");
    display.display();

    Serial.print("Wifi Connecting...");

    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    display.clearDisplay();
    display.setCursor(30, 5);
    display.print("WiFi connected");
    display.display();

    /* Initial with KEY, SECRET and also set the ALIAS here */
    microgear.init(KEY,SECRET,ALIAS);

    /* connect to NETPIE to a specific APPID */
    microgear.connect(APPID);
}

void loop() {
    /* To check if the microgear is still connected */
    if (microgear.connected()) {
        Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

        if (timer >= 1000) {
            float vhud = dht.readHumidity();
            float vtmp = dht.readTemperature();
            char ascii[32];

            if (isnan(vhud) || isnan(vtmp) || vhud > 100 || vtmp > 100){
                vhud = 0.0;
                vtmp = 0.0;
            }

            int humid_value = (vhud - (int)vhud) * 100;
            int tempe_value = (vtmp - (int)vtmp) * 100;

            Serial.print("Humidity: ");
            Serial.print(vhud);
            Serial.print(" %\t");
            Serial.print("Temperature: ");
            Serial.print(vtmp);
            Serial.print(" *C ");

            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setCursor(85, 5);
            display.setTextSize(1);
            display.print(vhud);
            display.setCursor(120, 5);
            display.println("%");
            display.setCursor(85, 20);
            display.print(vtmp);
            display.setCursor(120, 20);
            display.println("C");
            display.display();

            display.drawBitmap(60, 3,  waterDrop, 16, 10, 1);
            display.display();

            display.drawBitmap(60, 17,  thermo, 16, 11, 1);
            display.display();
            
            display.drawBitmap(0, 4,  gear, 48, 24, 1);
            display.display();

            sprintf(ascii,"%d.%d,%d.%d,d-duino", (int)vtmp, tempe_value,(int)vhud,humid_value);
            microgear.chat("duinosensor",ascii);
            timer = 0;
        } 
        else timer += 100;
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}
