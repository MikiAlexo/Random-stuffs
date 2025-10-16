#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <SPI.h>
#include <MFRC522.h>
#include <string.h>
#include <strings.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include"misc.h"
#include <time.h>

#define i2c_Address 0x3C
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ss_PIN 27
#define RST_PIN 2

MFRC522 mfrc522(ss_PIN, RST_PIN);

#define WIFI_SSID "Blynk"
#define WIFI_PASSWORD "12345678"

#define API_KEY "NUH-UH not giving away my key"
#define DATABASE_URL "https://esp32-firebase-demo-ed97b-default-rtdb.europe-west1.firebasedatabase.app/" 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

String uidString = "";
int totalPoints = 0;
int bonusPoints = 0;
int visitCount = 0;
int spaCount=0;
int poolCount=0;
int gymCount=0;
int boatCount=0;
int buzz_pin=13;
String membershipType = "";

struct tm timeinfo;
// String spa_path="v";

int waitForSerialPoints() {
  Serial.println("ENTER_POINTS");
  int input = 0;
  String Sinput="";
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n'){input = Sinput.toInt(); return input; break;}
      Sinput += c;
    }
  }
  int addedPoints = input;
  totalPoints += addedPoints;

  // Update membership type logic again
  if (totalPoints < 3000)
    membershipType = "Bronze";
  else if (totalPoints < 5000)
    membershipType = "Gold";
  else
    membershipType = "Platinum";
}

void setup() {
  Serial.begin(115200);
  pinMode(buzz_pin,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(500);
  display.begin(i2c_Address, true);

  display.clearDisplay();
  display.drawPixel(10, 10, SH110X_WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
  //bit map logo
 display.drawBitmap(0,0,  epd_bitmap_kuriftu_logo, 128, 64,SH110X_WHITE);
  display.display();
  delay(1);
  
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

 // Initialize and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");

  // Wait for time to be set
  
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

Serial.println(&timeinfo, "Time: %H:%M:%S");
  Serial.println(&timeinfo, "Date: %d/%m/%Y");

  config.token_status_callback = tokenStatusCallback; 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("connected to Firebase");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    delay(1000);
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card detected but failed to read UID");
    return;
  }

  uidString = ""; 
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }

  mfrc522.PICC_HaltA();

  String path = "/users/";
  String log="/users/";
  path.concat(uidString);
  log.concat(uidString);
  log.concat("/visit_logs");

  String totalPointsPath = path ;
  String bonusPointsPath = path ;
  String visitCountPath = path ;
  String membershipTypePath = path;
  String Spa_Count=log;
  String PoolCount=log;
  String GymCount=log;
  String BoatCount=log;
  String  spa_path=Spa_Count;

  totalPointsPath.concat("/total_points");
  bonusPointsPath.concat("/bonus_points");
  visitCountPath.concat("/visit_count");
  membershipTypePath.concat( "/membership_type");
  Spa_Count.concat("/spa_visits/");
  PoolCount.concat("/Pool_visits");
  GymCount.concat("/gym_visits");
  BoatCount.concat("/boat_trips");

  bool isNewUser = false;

  if (Firebase.RTDB.getInt(&fbdo, totalPointsPath)) {
    totalPoints = fbdo.to<int>();
    Firebase.RTDB.getInt(&fbdo, bonusPointsPath);
    bonusPoints = fbdo.to<int>();
    Firebase.RTDB.getInt(&fbdo, visitCountPath);
    visitCount = fbdo.to<int>();
    Firebase.RTDB.get(&fbdo, membershipTypePath);
    membershipType = fbdo.to<String>();
    Firebase.RTDB.getInt(&fbdo, Spa_Count);
    spaCount = fbdo.to<int>();
    Firebase.RTDB.getInt(&fbdo, PoolCount);
    poolCount = fbdo.to<int>();
    Firebase.RTDB.getInt(&fbdo, GymCount);
    gymCount = fbdo.to<int>();
    Firebase.RTDB.getInt(&fbdo, BoatCount);
    boatCount = fbdo.to<int>();

    visitCount += 1;
    spaCount+=1;
    totalPoints -= 500;
    if (totalPoints < 0){ 
      totalPoints = 0;
      display.clearDisplay();
      display.drawBitmap(0,0,  epd_bitmap_cross_kurif, 128, 64,SH110X_WHITE);
      display.display();
      decline_tone();
      delay(2000);
      display.clearDisplay();
      display.drawBitmap(0,0,  epd_bitmap_kuriftu_logo, 128, 64,SH110X_WHITE);
      display.display();
      delay(1);}
    else{
      display.clearDisplay();
      display.drawBitmap(0,0,  epd_bitmap_checked_kurif, 128, 64,SH110X_WHITE);
      display.display();
      pass_tone();
      delay(1600);
      display.clearDisplay();
      display.drawBitmap(0,0,  epd_bitmap_kuriftu_logo, 128, 64,SH110X_WHITE);
      display.display();
      delay(1);
    }
    bonusPoints += ((totalPoints==0?0:totalPoints) * 1) / 100;

    if (totalPoints < 25000)
      membershipType = "Bronze";
    else if (totalPoints >= 25000 && totalPoints <= 75000)
      membershipType = "Silver";
    else if(totalPoints >= 75000 && totalPoints <= 150000)
      membershipType = "Gold";
    else if(totalPoints >= 150000 && totalPoints > 150000)
       membershipType = "Platinuim";

    if(membershipType=="Bronze")
        bonusPoints += ((totalPoints==0?0:totalPoints) * 1) / 100;
    else if (membershipType=="Silver")
        bonusPoints += ((totalPoints==0?0:totalPoints) * 5) / 100;
    else if(membershipType=="Gold")
        bonusPoints += ((totalPoints==0?0:totalPoints) * 10) / 100;
    else if(membershipType=="Platinium")
        bonusPoints += ((totalPoints==0?0:totalPoints) * 20)/ 100;

int currentHour, currentMinute;
int currentDay, currentMonth, currentYear;

currentHour   = timeinfo.tm_hour;
  currentMinute = timeinfo.tm_min;


  currentDay    = timeinfo.tm_mday;
  currentMonth  = timeinfo.tm_mon + 1;        
  currentYear   = timeinfo.tm_year + 1900;

 String date=(String)currentDay;date.concat("|");date.concat(currentMonth);  date.concat("|"); date.concat(currentYear);date.concat("/");
 spa_path=Spa_Count;
spa_path.concat(date);
String time_date= (String)currentHour;time_date.concat(":"); time_date.concat(currentMinute);time_date.concat("/");
spa_path.concat(time_date);
Serial.println(spa_path);
  } else {
    isNewUser = true;
    totalPoints = 1000;
    bonusPoints = 0;
    visitCount = 1;
    membershipType = "Bronze";
    spaCount=1;
    poolCount=0;
    gymCount=0;
    boatCount=0;

  }



  // Save updates
  Firebase.RTDB.setInt(&fbdo, visitCountPath, visitCount);
  Firebase.RTDB.setInt(&fbdo, totalPointsPath, totalPoints);
  Firebase.RTDB.setInt(&fbdo, bonusPointsPath, bonusPoints);
  Firebase.RTDB.setString(&fbdo, membershipTypePath, membershipType);
  // Firebase.RTDB.setInt(&fbdo, spa_path, spaCount);
  Firebase.RTDB.setInt(&fbdo, Spa_Count, spaCount);
  Firebase.RTDB.setInt(&fbdo, PoolCount, poolCount);
  Firebase.RTDB.setInt(&fbdo, GymCount, gymCount);
  Firebase.RTDB.setInt(&fbdo, BoatCount, boatCount);

  if (isNewUser) {
    Serial.println("NEW_USER_CREATED");
  } else {
    Serial.println("UPDATED");
  }

  delay(1000); 
}

void pass_tone(){
digitalWrite(buzz_pin,HIGH);
delay(500);
digitalWrite(buzz_pin,LOW);
delay(200);
digitalWrite(buzz_pin,HIGH);
delay(500);
digitalWrite(buzz_pin,LOW);
delay(200);

}
void decline_tone(){
digitalWrite(buzz_pin,HIGH);
delay(1000);
digitalWrite(buzz_pin,LOW);
}


void log(){




}
