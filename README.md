# NETPIE-Dduino
NETPIE-Dduino คือ โค๊ดการทำงาน เพื่อใช้สร้าง Smart Sensor ที่สามารถแจ้งค่าอุณหภูมิและความชื้นได้แบบ Realtime โดยใช้ Board [D-Duino](https://www.gravitechthai.com/product_detail.php?d=1362)  จาก Home of Maker ร่วมกับ Sensor [DHT22](https://www.adafruit.com/product/385) ในการใช้วัดอุณหภูมิและความชื้น นำมาสร้างเป็นอุปกรณ์ IoT โดย [NETPIE](https://netpie.io)

**สิ่งที่ต้องมี**
- Board D-Duino
- Sensor DHT22
- สาย Micro USB (เพื่อใช้เชื่อมต่อ Board เข้ากับเครื่องคอมพิวเตอร์)
- โปรแกรม Arduino IDE

**Library ที่ใช้**
- Adafruit GFX
- Adafruit SSD1306
- DHT sensor library
- ESP8266 Microgear
<br>// สามารถโหลดได้ในโปรแกรม Arduino IDE โดยค้นหาชื่อที่หน้าต่าง Library Manager และกด Install
- esp8266 จาก Board Manager

** การต่อสาย DHT22 กับ D-duino **

(DHT22 ฝั่งซ้าย / D-duino ฝั่งขวานะครับ)
- Pin 1 : 5V หรือ 3V
- Pin 2 : D3
- Pin 3 : ไม่ต่ออะไร
- Pin 4 : GND

<<<<<<< HEAD
และตัวต้านทานดังภาพ https://www.img.in.th/image/7ARB
=======
และตัวต้านทานต่อระหว่าง Pin 1 และ Pin 2
>>>>>>> dev-sek

**ค่าที่ต้องแก้**
```
const char* ssid     = "ssid"; // ชื่อ Wifi ที่ใช้เชื่อมต่อ
const char* password = "pass"; // Password wifi

#define APPID   "APPID" // appid ที่ตั้งในเว็บ netpie.io
#define KEY     "KEY" // key ที่ได้จากการสร้าง device key หรือ session key
#define SECRET  "SECRET" // secret ที่ได้จากการสร้าง device key หรือ session key
#define ALIAS   "d-duino" // ตั้งชื่ออุปกรณ์ (ไม่จำเป็นต้องตั้งก็ได้)
#define DHTPIN  0 // pin ที่เชื่อม D-Duino กับ DHT22
#define DHTTYPE DHT22 // อาจแก้เป็น DHT11 หรือ DHT21 ตาม sensor ที่ท่านใช้
```
