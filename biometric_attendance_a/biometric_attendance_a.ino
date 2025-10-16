#include "DS3232Minimal.h"
#include "SD_Minimal.h"
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
DS3232Minimal rtc;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);

SD_Minimal sd (53);


uint8_t id;
String monthsOfTheYear [12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
String title;
String Rtitle;
String logPath;
String absent;
boolean enroll = false;
boolean en = false;
int compareidd = 0;
int compareid = 0;
int resetbut = 40;
int timein1 = 8 ;
int timein2 = 9;
int timeout1 = 16;
int timeout2 = 18;
long lastTime, period = 1000;

void setup () {
  Serial.begin (9600);
  finger.begin(57600);
  rtc.initialize ();
  delay (500);

  //title of the data
  title = monthsOfTheYear [month () - 1];
  logPath = title + String (day ()) + ".txt";
  Rtitle = "dataBase.txt";
  absent = "absent" + String(day()) + ".txt";
  lcd.init();                      // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 4);
  delay(100);

  pinMode(resetbut, INPUT_PULLUP);

  Serial.println (logPath);
  //sets the time for the rtc
  //rtc.setTimeDate(10,15,0,20,9,2022);

  //checks if the sensor is operating
  if (finger.verifyPassword()) {
    lcd.print("sensor found!");
    tone(8, 2794, 200);
    delay(100);
    tone(8, 2093, 200);
    delay(200);
    tone(8, 3520, 250);
    delay(100);
    noTone(8);

  } else {
    lcd.print("Did not find fingerprint sensor,please check your wires");
    tone(8, 2093, 1000);
    delay(100);
    tone(8, 293, 1000);
    delay(100);
    noTone(8);
    while (1) {
      delay(1);
    }
  }


  lcd.clear();
  finger.getTemplateCount();
  Serial.print("sensor contains:");
  Serial.print(finger.templateCount);
  Serial.println("finger registered");
  delay(100);

  lcd.clear();
  sd.begin ();

  //  sd.deleteFile (logPath);
  //  delay (300);

  //checks if the file exist
  if (!sd.fileExists (logPath))
    sd.createOrAppend (logPath, "ID , TimeIN , TimeOut\t\t" + String(rtc.dateStamp()) + "\n");
  if (!sd.fileExists(Rtitle))
    sd.createOrAppend (Rtitle  , "FULL NAME , ID \n");

  String contents = sd.getContent (logPath);
  //  String content = getValue (contents, '\n', 0);

  Serial.println ("Contents:");
  Serial.println (contents);
  //sd.replaceLineWith (logPath, 0, content + "16:23");


  //  if (countChars(content, ',') == 2)
  //    sd.save(logPath, hour + ":" + minute);
  //  else if (countChars(contents, ',') == 3)
  //    sd.save(logPath, hour + ":" + minute);
  //  else
  //    return;

  lastTime = millis ();
}
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}




// serial communication
void listener () {
  while (Serial.available() > 0) {
    String pid = Serial.readStringUntil('\n');
    pid.replace("\n", "");
    pid.replace ("\r", "");
    Serial.println(pid);


    if (pid.startsWith("*") && pid.endsWith ("*")) {
      pid.replace ("*", "");
      String name = getValue (pid, ',', 0);
      id = getValue (pid, ',', 1).toInt ();
      Serial.print ("_");
      Serial.print (name);
      Serial.print (",");
      Serial.print (id);
      Serial.println ("_");
      enroll = true;

      lcd.clear();
      lcd.print("Ready to enroll!");
      delay(1000);
      lcd.clear();
      lcd.print("Enrolling ID ");
      lcd.print(id);
      Serial.println(id);
      delay(1000);
      lcd.clear();

      boolean c = getFingerprintEnroll();
      if (c) {
        sd.save(Rtitle, name + "," + String (id));
        Serial.println ("Saved");
      }
    }
    else if (pid.startsWith("4365")) {
      pid.replace("4365", "");
      id = pid.toInt();
      lcd.print("Deleting ID #");
      Serial.println(id);
      deleteFingerprint(id);
      id = 0;
    }

    else if (pid.startsWith("!") && pid.endsWith("!")) {
      pid.replace("!", "");
      int shour = getValue (pid, ',', 0).toInt();
      int sminute = getValue (pid, ',', 1).toInt();
      int sday = getValue(pid, ',', 2).toInt();
      int smonth = getValue (pid, ',', 3).toInt();
      int syear = getValue (pid, ',', 4).toInt();
      rtc.setTimeDate(shour, sminute, 0, sday, smonth, syear);
    }
    else if (pid.startsWith("%") && pid.endsWith("%")) {
      pid.replace("%", "");
      int linhour = getValue(pid, ',', 0).toInt();
      int linminute = getValue(pid, ',', 1).toInt();
      int louthour = getValue(pid, ',', 2).toInt();
      int loutminute = getValue(pid, ',', 3).toInt();

      timein1 = linhour;
      timein2 = linminute;
      timeout1 = louthour;
      timeout2 = loutminute;


    }
  }
}

// resets the device
void (*reset)(void) = 0;

void loop () {
  if (digitalRead(resetbut) == 0) {
    while (digitalRead(resetbut) == 0);
    reset();
  }
  while (!sdf) {
    if (millis () - lastTime > period) {
      lcd.clear();
      lcd.print ("   " + rtc.dateStamp () + "  ");
      lcd.print (" ");
      lcd.print ("  " + rtc.timeStamp ());
      tone(8, 1760, 250);
      delay(100);
      noTone(8);
      lcd.setCursor(-2, 3);
      lcd.print("enter sd card");

      lastTime = millis ();
    }
    //    lcd.setCursor(3, 3) ;
    //    lcd.print("" + rtc.namedate());
    listener ();
  }

  //checks if it's not enrolling
  while (!enroll) {
    //checks time
    if (rtc.timeStamp ().toInt() >= timein1 && rtc.timeStamp ().toInt() <= timein2/*18-9*/ || rtc.timeStamp ().toInt() >= timeout1 && rtc.timeStamp ().toInt() <= timeout1) {
      getFingerprintID();
      listener ();
      enroll = false;
    } else if (/*rtc.timeStamp ().toInt() >= 19 && rtc.timeStamp ().toInt() <= 20*/ true || rtc.timeStamp ().toInt() >= 8 && rtc.timeStamp ().toInt() <= 16 ) {
      checkabsent();
      listener ();
    }
    else {
      lcd.clear();
      lcd.print("On time limit");
      delay(500);

    }

  }
}


//      for (int c = 1; c < fileLength(Rtitle); c++) {
//        String dataline = getValue(datacont, '\n', c);
//
//        String cidd = getValue(dataline, ',', 1);
//        int compareidd = cidd.toInt();
//
//      }
//      for (int a = 1; a < fileLength(logPath); a++) {
//        String content = getValue(contents, '\n', a);
//        String cid = getValue(content, ',', 0);
//        compareid = cid.toInt();
//      }
//      if (compareidd != compareid){
//        String cidd = String(compareidd);
//        sd.save(absent, cidd);
//      }
