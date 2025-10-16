
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(2);

int capture_but = 18;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup() {
  Serial.begin(115200);//115200
  mySerial.begin(57600);

  pinMode(capture_but, INPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);     
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0);
  display.print("rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)config:0, SPIWP:0xeeclk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00mode:DIO, clock di");
  display.display();
  delay(150);
  display.clearDisplay();
}

void loop() {
  if (digitalRead(capture_but) == 1) {
    //    Serial.println(digitalRead(capture_but));
    delay(100);
    finimg();
    //  Serial.println("capturing finger image");
  }
  if (mySerial.available()) {
    display.clearDisplay();
    display.println("sending img");
    while (mySerial.available()) {
      byte data = mySerial.read();
      Serial.print(data, HEX);
      Serial.print(" ");
      display.print(data);
      //      hexData+=endMarker;
      //      incoming_data = true;
      //      if (incoming_data = true) {
      //        //hexData+=startMarker;
      //        hexData += (data, HEX);
      //      }
      //      String imageData = extractImageData(hexData);
      //      if (imageData != "") {
      //        Serial.println("Extracted Image Data:");
      //        Serial.println(imageData);
      //      }
      //  Serial.print(" ");
    }
    //Serial.println();
  }
  display.clearDisplay();
}

void finimg() {
  if (captureFingerprintImage()) {
    Serial.println("Image captured successfully.");
  } else {
    Serial.println("Failed to capture image.");
  }


  if (uploadFingerprintImage()) {
    Serial.println("Image uploaded successfully.");
  } else {
    Serial.println("Failed to upload image.");
  }
}
bool captureFingerprintImage() {
 
  byte captureCmd[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x01, 0x00, 0x05};
  mySerial.write(captureCmd, sizeof(captureCmd));

 
  delay(1000);

   
  if (mySerial.available()) {
    byte response[12];
    mySerial.readBytes(response, 12);
    if (response[9] == 0x00) {
      return true;
    }
  }
  return false;
}

bool uploadFingerprintImage() {
  byte uploadCmd[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x0A, 0x00, 0x0E};
  //(header,adress,packet identifier,pakage length,instruction code,parameter(buffer0x00 or default),checksum))
  mySerial.write(uploadCmd, sizeof(uploadCmd));

 
  delay(1000);

  // Read response
  if (mySerial.available()) {
    byte response[12];
    mySerial.readBytes(response, 12);
    if (response[9] == 0x00) {
      return true;
    }
  }
  return false;
}
