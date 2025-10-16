//this function is used for checking finger
int fileLength (String filePath) {
  File file = SD.open(filePath, O_READ);

  if (!file)
    return -1;

  int counter = 0;

  while (file.available ()) {
    file.readStringUntil ('\n');
    counter ++;
  }
  file.close();

  return counter;
}

void replaceLineWith (String filePath, int line, String contentToReplace) {
  int lines = fileLength (filePath);

  if (lines == -1)
    return;

  File file = SD.open(filePath, O_READ);

  int counter = 0;

  String contents [lines];
  String eachContent;

  while (file.available ()) {
    eachContent = file.readStringUntil ('\n');

    String clean = "";
    for (int a = 0;  a < eachContent.length () - 1; a ++) clean += eachContent [a];

    if (line == counter)
      contents [counter] = contentToReplace;
    else
      contents [counter] = clean;

    counter ++;
  }
  file.close();

  SD.remove (filePath);
  delay (10);

  file = SD.open(filePath, FILE_WRITE);
  for (int a = 0; a < lines; a ++)
    file.println (contents [a].c_str ());
  file.close();
}



uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image taken");
      delay(1000);
      lcd.clear();
      break;
    case FINGERPRINT_NOFINGER:
      lcd.setCursor(-2, 1);
      lcd.print("  Place finger");
      delay(500);
      lcd.clear();
      lcd.setCursor(-4, 3);
      lcd.print("Mode:attend");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.print("Communication error");
      tone(8, 2093, 1000);
      delay(100);
      tone(8, 293, 1000);
      delay(100);

      return p;
    case FINGERPRINT_IMAGEFAIL:
      lcd.print("Imaging error");
      return p;
    default:
      lcd.print("Unknown error");
      return p;

  }

  // OK success

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Please wait...");
      delay(1000);
      lcd.clear();
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.print("Communication error");
      tone(8, 2093, 1000);
      delay(100);
      tone(8, 293, 1000);
      delay(100);
      //noTone(8);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.print("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.print("Could not find fingerprint features");
      return p;
    default:
      lcd.print("Unknown error");
      return p;
  }

  // OK converted
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    lcd.print("Found a match!");
    tone(8, 3520, 200);
    delay(250);
    tone(8, 3520, 200);
    noTone(8);
    delay(500);
    lcd.clear();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.print("Communication error");
    tone(8, 2093, 1000);
    delay(100);
    tone(8, 293, 1000);
    delay(100);
    //noTone(8);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    lcd.print("Didn't find a match");
    tone(8, 2000, 200);
    delay(200);
    noTone(8);
    delay(3000);
    lcd.clear();
    return p;
  } else {
    lcd.print("Unknown error");
    return p;
  }

  // found a match
  uint8_t ID = finger.fingerID;
  lcd.print("with id#");
  lcd.print(ID);
  delay(2000);
  lcd.clear();
  if (ID <= 0)
    return;
  //  String Dcontents = sd.getContent (Rtitle);

  String todaysList = sd.getContent (logPath);
  int lines = countChars (todaysList, '\n');


  Serial.print ("Line count:");
  Serial.println (lines);
  String timeStamp = rtc.timeStamp ();
  timeStamp.replace ("\n", "");
  if (lines == 1) {
    String line = String (ID) + "," + timeStamp;
    sd.createOrAppend (logPath, line + "\n");
  } else {
    cleanFile (logPath);
    Serial.print ("Attended Before: ");
    boolean attended = attendedBefore (ID, logPath);
    Serial.println (attended);

    if (!attended) {
      Serial.println ("Saving TimeIN");
      lcd.clear();
      lcd.print("Saving TimeIN");
      delay(100);
      String line = String (ID) + "," + timeStamp + "\n";

      sd.createOrAppend (logPath, line);
    } else {
      Serial.println ("Saving TimeOUT");
      lcd.clear();
      lcd.print("saving TimeOUT");
      delay(100);
      int lineIndex = getIDIndex (ID, logPath);
      String line = getValue (todaysList, '\n', lineIndex);
      int commas = countChars (line, ',');

      if (commas == 1) {
        line.replace ("\n", "");
        line += "," + timeStamp;
        sd.replaceLineWith (logPath, lineIndex, line + "\n");
      } else {
        Serial.println ("Already logged TimeOUT.");
        lcd.clear();
        lcd.print("Already logged TimeOUT");
        delay(100);
      }
    }
  }
  return finger.fingerID;
}

void cleanFile (String path) {
  String todaysList = sd.getContent (path);
  String cleaned = "";
  int lines = countChars (todaysList, '\n');

  for (int a = 0; a < lines; a ++) {
    String line = getValue (todaysList, '\n', a);
    line.trim ();
    if (line.length () > 0)
      cleaned += line + "\n";
  }

  sd.deleteFile (path);
  delay (200);
  sd.createOrAppend (path, cleaned);
}

boolean attendedBefore (int checkID, String path) {
  String todaysList = sd.getContent (path);
  int lines = countChars (todaysList, '\n');

  for (int a = 1; a < lines; a ++) {
    String line = getValue (todaysList, '\n', a);
    String IDstr = getValue (line, ',', 0);

    int ID = IDstr.toInt ();

    if (checkID == ID) return true;
  }

  return false;
}
int getIDIndex (int checkID, String path) {
  String todaysList = sd.getContent (path);
  int lines = countChars (todaysList, '\n');

  for (int a = 1; a < lines; a ++) {
    String line = getValue (todaysList, '\n', a);
    String IDstr = getValue (line, ',', 0);

    int ID = IDstr.toInt ();

    if (checkID == ID) return a;
  }

  return -1;
}
