//function to check absent
void checkabsent() {
  if (!sd.fileExists(absent))
    sd.save(absent, "list of absent employees \n");

  String dataLines = sd.getContent(Rtitle);
  String logLines = sd.getContent(logPath);
  String absentLines = sd.getContent(absent);

  Serial.print ("Data Lines:");
  Serial.println (dataLines);

  Serial.print ("Log Lines:");
  Serial.println (logLines);

  Serial.print ("Absent Lines:");
  Serial.println (absentLines);

  String dataIDs = getDataId(Rtitle);
  dataIDs.replace ("\n", "");
  Serial.print ("Data IDs: ");
  Serial.println (dataIDs);

  String logIDs = getLogId(logPath);
  logIDs.replace ("\n", "");
  Serial.print ("Log IDs: ");
  Serial.println (logIDs);

  int dataCount = countChars (dataIDs, ',');
  int logCount = countChars (logIDs, ',');
  int numberOfAbsentees = dataCount - logCount;

  Serial.print ("No. of Absentees: ");
  Serial.println (numberOfAbsentees);

  String listOfAbsentIDs = "";
  String listOfAbsentNames = "";
  for (int a = 0; a < dataCount + 1; a ++) {
    String dataID = getValue (dataIDs, ',', a);
    boolean present = false;

    for (int b = 0; b < logCount + 1; b ++) {
      String logID = getValue (logIDs, ',', b);

      if (dataID.equals (logID)) {
        present = true;
        break;
      }
    }

    if (!present) {
      if (!listOfAbsentNames.equals ("")) listOfAbsentNames += "\n";
      listOfAbsentNames += getNameFromID (Rtitle, dataID.toInt ());
    }
  }

  Serial.println ("List of absentees:");
  Serial.println (listOfAbsentNames);
  String savedAbsentees = sd.getContent (absent);
  if (!contains (savedAbsentees, listOfAbsentNames)) {
    listOfAbsentNames = "list of absent employees \n" + listOfAbsentNames;
    sd.deleteFile (absent);
    delay (200);
    Serial.println ("Saving absentees");
    sd.save (absent, listOfAbsentNames);
  }
}
