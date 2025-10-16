#include <HardwareSerial.h>

HardwareSerial mySerial(2);

int capture_but = 18;

const char* startMarker = "EF1FFFFFFFF";
const char* endMarker = "10";
boolean incoming_data = false;

String hexData = "";

String extractImageData(String hexData) {
  int startIndex = hexData.indexOf(startMarker);
  if (startIndex == -1) {
    Serial.println("Start marker not found.");
    return "";
  }
  startIndex += strlen(startMarker);


  int endIndex = hexData.indexOf(endMarker, startIndex);
  if (endIndex == -1) {
    Serial.println("End marker not found.");
    return "";
  }

  String imageData = hexData.substring(startIndex, endIndex);

  return imageData;
}
void setup() {
  Serial.begin(115200);
  mySerial.begin(57600);

  pinMode(capture_but, INPUT);

}

void loop() {
  if (digitalRead(capture_but) == 1) {
    hexData+=startMarker;
    //    Serial.println(digitalRead(capture_but));
    delay(100);
    finimg();
    //  Serial.println("capturing finger image");
  }
  if (mySerial.available()) {
    while (mySerial.available()) {
      byte data = mySerial.read();
      //Serial.print(data, HEX);
//      hexData+=endMarker;
      incoming_data = true;
      if (incoming_data = true) {
        //hexData+=startMarker;
        hexData += (data,HEX);
      }
      String imageData = extractImageData(hexData);
      if (imageData != "") {
        Serial.println("Extracted Image Data:");
        Serial.println(imageData);
      }
      //  Serial.print(" ");
    }
    //Serial.println();
  }
}

void finimg() {
  if (captureFingerprintImage()) {
    // Serial.println("Image captured successfully.");
  } else {
    // Serial.println("Failed to capture image.");
  }

  if (uploadFingerprintImage()) {
    //Serial.println("Image uploaded successfully.");
  } else {
    //    Serial.println("Failed to upload image.");
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

  if (mySerial.available()) {
    byte response[12];
    mySerial.readBytes(response, 12);
    if (response[9] == 0x00) {
      return true;
    }
  }
  return false;
}
void decode_(String image) {

}
