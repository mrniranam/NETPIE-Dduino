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
float vhudOld = 0.0;
float vtmpOld = 0.0;

MicroGear microgear(client);
DHT dht(DHTPIN, DHTTYPE);

static const unsigned char PROGMEM gear[] =
{ 
0x0, 0x0, 0x7, 0xc0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x3, 0xff, 
0xc0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xe0, 0x1c, 0x0, 
0x0, 0x7f, 0xff, 0xe0, 0x3e, 0x0, 0x0, 0x7f, 0xff, 0xf0, 0x7f, 0x80, 0x0, 0x7f, 0xff, 
0xf8, 0xff, 0xc0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xe0, 
0x0, 0x3f, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x1f, 0xff, 0xff, 0xff, 0x80, 0x0, 0x1f, 0xff, 
0xff, 0xfe, 0x8, 0x0, 0x3f, 0xff, 0xff, 0xfc, 0x3c, 0x0, 0x3f, 0xf0, 0x1f, 0xf8, 0x7c, 
0x3f, 0xff, 0xc0, 0x7, 0xe1, 0xfe, 0x7f, 0xff, 0x80, 0x1, 0xc3, 0xfe, 0x7f, 0xff, 0x7, 
0xe0, 0x8f, 0xfe, 0x7f, 0xff, 0x1f, 0xf8, 0x1f, 0xff, 0x7f, 0xfe, 0x3f, 0xfc, 0x3f, 0xff, 
0xff, 0xfe, 0x3f, 0xfc, 0xff, 0xfe, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xf8, 0xff, 0xfc, 0x7f, 
0xff, 0xff, 0xe0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xc0, 
0x0, 0x0, 0x7f, 0xff, 0xff, 0xc0, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xc0, 0xff, 0xfe, 0x3f, 
0xfc, 0xff, 0xf0, 0x7f, 0xfe, 0x1f, 0xf8, 0x7f, 0xf8, 0x7f, 0xff, 0xf, 0xf0, 0x3f, 0xfe, 
0x7f, 0xff, 0x83, 0xc0, 0x1f, 0xfe, 0x7f, 0xff, 0xc0, 0x3, 0x7, 0xfe, 0x3f, 0xff, 0xf0, 
0xf, 0xc3, 0xfe, 0x0, 0x7f, 0xfe, 0x7f, 0xe1, 0xfc, 0x0, 0x3f, 0xff, 0xff, 0xf0, 0x7c, 
0x0, 0x1f, 0xff, 0xff, 0xfc, 0x38, 0x0, 0x3f, 0xff, 0xff, 0xfe, 0x18, 0x0, 0x3f, 0xff, 
0xff, 0xff, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x0, 0x7f, 0xff, 0xff, 0xff, 0xc0, 
0x0, 0x7f, 0xff, 0xfc, 0xff, 0xc0, 0x0, 0x7f, 0xff, 0xf0, 0x7f, 0x80, 0x0, 0x7f, 0xff, 
0xe0, 0x3f, 0x0, 0x0, 0x3f, 0xff, 0xe0, 0x1c, 0x0, 0x0, 0xf, 0xff, 0xe0, 0x8, 0x0, 
0x0, 0x3, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0xf, 
0xc0, 0x0, 0x0, 
};

static const unsigned char PROGMEM waterDrop[] =
{ 0x0, 0x0, 0x0, 0x80, 0x1, 0x80, 0x1, 0x80, 0x3, 0xc0, 0x3, 0xc0, 0x7, 0xe0, 0x7, 
0xf0, 0xf, 0xf0, 0x1f, 0xf8, 0x1f, 0xf8, 0x3f, 0xfc, 0x3f, 0xfe, 0x7f, 0xfa, 0x7f, 0xfb, 
0xff, 0xf9, 0xff, 0xf9, 0xff, 0xf9, 0xff, 0xf9, 0x7f, 0xfb, 0x7f, 0xfa, 0x7f, 0xfe, 0x3f, 
0xfc, 0x1f, 0xf8, 0xf, 0xf0, 0x3, 0xc0, };

static const unsigned char PROGMEM thermo[] =
{ 
  0x1e, 0xe, 0x3f, 0x1f, 0x33, 0x93, 0x61, 0x93, 0x61, 0x9f, 0x7f, 0x8e, 0x61, 0x80, 0x61, 
0x80, 0x61, 0x80, 0x61, 0x80, 0x7f, 0x80, 0x61, 0x80, 0x61, 0x80, 0x61, 0x80, 0x7f, 0x80, 
0x7f, 0x80, 0x61, 0x80, 0x61, 0x80, 0x61, 0x80, 0x7f, 0x80, 0x71, 0x80, 0x61, 0xc0, 0xc0, 
0xc0, 0xe0, 0xc0, 0xff, 0xc0, 0xff, 0xc0, 0x7f, 0xc0, 0x7f, 0x80, 0x1f, 0x0 };

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
    display.clearDisplay(); // clears the screen and buffer
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

            display.drawBitmap(57, 3,  waterDrop, 16, 26, 1);
            display.display();
          
            display.drawBitmap(60, 35,  thermo, 16, 29, 1);
            display.display();
            
            display.drawBitmap(3, 8,  gear, 48, 48, 1);
            display.display();
            
            Serial.print("Humidity: ");
            Serial.print(vhud);
            Serial.print(" %\t");
            Serial.print("Temperature: ");
            Serial.print(vtmp);
            Serial.print(" *C ");

            if(vhud != vhudOld || vtmp != vtmpOld){
              display.setTextColor(BLACK);
              display.setCursor(85, 15);
              display.setTextSize(1);
              display.print(vhudOld);
              display.setCursor(120, 15);
              display.setCursor(85, 47);
              display.print(vtmpOld);
              display.setCursor(120, 47);
              display.display();
    
              vhudOld = vhud;
              vtmpOld = vtmp;
    
              display.setTextColor(WHITE);
              display.setCursor(85, 15);
              display.setTextSize(1);
              display.print(vhud);
              display.setCursor(120, 15);
              display.println("%");
              display.setCursor(85, 47);
              display.print(vtmp);
              display.setCursor(120, 47);
              display.println("C");
              display.display();
            }

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
