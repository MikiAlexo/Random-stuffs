//this function is used for enrolling new users
uint8_t getFingerprintEnroll() {
  int p = -1;
  lcd.print("enrolling as:"); Serial.print(id); lcd.print(id);
  tone(8,3520,260);
  delay(100);
  tone(8,3520,260);
  delay(100);
  noTone(8);
  delay(500);
  lcd.clear();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.print("Image taken");
        delay(1000);
        lcd.clear();
        break;
      case FINGERPRINT_NOFINGER:
        lcd.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.print("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.print("Imaging error");
        break;
      default:
        lcd.print("Unknown error");
        break;
    }
  }
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Image converted");
      delay(1000);
      lcd.clear();
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.print("Communication error");
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
  lcd.print("Remove finger");
  delay(1000);
  lcd.clear();
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  lcd.print("Place finger again");
  delay(1000);
  lcd.clear();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        lcd.print("Image taken");
        delay(1000);
        lcd.clear();
        break;
      case FINGERPRINT_NOFINGER:
        lcd.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.print("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.print("Imaging error");
        break;
      default:
        lcd.print("Unknown error");
        break;
    }
  }
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.print("Image converted");
      delay(1000);
      lcd.clear();
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.print("Communication error");
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

  lcd.print("Creating model for:");  Serial.print(id); lcd.print(id);
  delay(1000);
  lcd.clear();
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.print("Prints matched!");
    delay(1000);
    lcd.clear();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.print("Communication error");
       tone(8, 2093, 1000);
    delay(100);
    tone(8, 293, 1000);
    delay(100);
    //noTone(8);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.print("Fingerprints did not match");
    tone(8, 2000, 200);
    delay(200);
    noTone(8);
    delay(3000);
    return p;
  } else {
    lcd.print("Unknown error");
    return p;
  }


  lcd.print("   ID "); lcd.print(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.print(" Stored!");
    lcd.setCursor(-2, 3);
    //lcd.print("now press but1");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.print("Communication error");
       tone(8, 2093, 1000);
    delay(100);
    tone(8, 293, 1000);
    delay(100);  
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.print("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.print("Error writing to flash");
    return p;
  } else {
    lcd.print("Unknown error");
    return p;
  }

  return true;
}
