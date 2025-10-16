#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4);
boolean sdf = false;


class SD_Minimal {
  public:
    unsigned int CS;
    double replacePercentageLimit = 99; // Percentage Limit To Move And Replace Log
    long totalCardSize;

    boolean available;

    SD_Minimal (unsigned int CS) {
      this -> CS = CS;
    }

    void begin () {
      totalCardSize = cardSize ();

      if (!SD.begin(CS)) {
        lcd.clear();
        lcd.print("SD Card not found");
        tone(8, 2093, 1000);
        delay(100);
        tone(8, 293, 1000);
        delay(100);
        tone(8, 2093, 1000);
        delay(100);
        tone(8, 293, 1000);
        delay(100);
        noTone(8);
        delay(1000);
      } else {
        lcd.clear();
        lcd.print("SD Card Found");
        tone(8, 3520, 250);
        delay(100);
        noTone(8);
        tone(8, 3520, 250);
        delay(100);
        noTone(8);
        sdf = true;
        delay(1000);
      }
    }

    double usedSpaceInPercent () {
      return longMap (usedSpace (), 0, totalCardSize, 0, 100);
    }

    double longMap (double val, double minI, double maxI, double minO, double maxO) {
      return minO + val * (maxO - minO) / (maxI - minI);
    }

    long cardSize () {
      SdVolume volume;
      Sd2Card card;

      if (card.init(SPI_HALF_SPEED, CS)) {
        if (volume.init(card)) {
          uint32_t volumesize;
          volumesize = volume.blocksPerCluster();
          volumesize *= volume.clusterCount();
          volumesize /= 2;
          return volumesize * 1024;
        }
      }

      return -1;
    }

    long usedSpace () {
      File root = SD.open("/");
      return dirSize (root);
    }

    long dirSize (File dir) {
      long fileSize = 0;

      while (true) {

        File entry =  dir.openNextFile();
        if (!entry) break;

        if (entry.isDirectory())
          fileSize += dirSize(entry);
        else
          fileSize += entry.size();

        entry.close();
      }

      return fileSize;
    }

    void save (String filePath, String content) {
      if (usedSpaceInPercent () < replacePercentageLimit)
        createOrAppend (filePath, content);
      else
        moveUpAndReplace (filePath, content);
    }

    bool createOrAppend (String filePath, String content) {
      File file = SD.open(filePath, FILE_WRITE);
      if (file) {
        file.print (content);
        file.close();
        return true;
      }
      return false;
    }

    void displayContent (String filePath) {
      File file = SD.open(filePath, O_READ);

      if (file) {
        while (file.available ())
          Serial.println (file.readStringUntil ('\n'));
        file.close();
      }
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

    void deleteFile (String filePath) {
      SD.remove (filePath);
      delay (10);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////
    void deleteline(String path , int index) {
      String empty;
      replaceLineWith (path, index, empty);
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////
    void moveUpAndReplace (String filePath, String contentToReplace) {
      int lines = fileLength (filePath);

      if (lines == -1)
        return;

      File file = SD.open(filePath, O_READ);

      int counter = 0;

      String contents [lines];
      String eachContent;
      bool firstLine = true;

      while (file.available ()) {
        eachContent = file.readStringUntil ('\n');
        if (firstLine) {
          firstLine = false;
          continue;
        }

        String clean = "";
        for (int a = 0;  a < eachContent.length () - 1; a ++) clean += eachContent [a];

        contents [counter ++] = clean;
      }
      file.close();

      contents [counter] = contentToReplace;

      SD.remove (filePath);
      delay (10);

      file = SD.open(filePath, FILE_WRITE);
      for (int a = 0; a < lines; a ++)
        file.println (contents [a].c_str ());
      file.close();
    }

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

    String getContent (String filePath) {
      File file = SD.open(filePath, O_READ);

      String content = "";
      if (file) {
        while (file.available ())
          content += file.readStringUntil ('\n') + "\n";
        file.close();
        return content;
      }
    }

    bool fileExists (String filePath) {
      return SD.exists (filePath);
    }
};
