char separator = ',';

boolean contains (String within, String lookFor) {
  return strstr((const char*) within.c_str(), lookFor.c_str()) != NULL;
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int countChars (String data, char separator) {
  int counter = 0;

  for (int a = 0; a < data.length(); a ++)
    if (data [a] == separator)
      counter ++;

  return counter;
}

float extractVal (String data, String key) {
  float val;
  data.replace (key, "");
  data.trim ();

  if (data.length () > 0)
    return atof (data.c_str ());
  else
    return -1;
}

String getNameFromID (String path, int id) {
  String contents = sd.getContent (path);
  int length = fileLength (path);

  for (int a = 1; a < length; a ++) {
    String content = getValue (contents, '\n', a);
    String name = getValue (content, ',', 0);
    String idStr = getValue (content, ',', 1);

    if (idStr.toInt () == id) {
      return name;
    }
  }
  return "";
}

//String getAbsentIDs (String path) {
//  Serial.print ("Path: ");
//  Serial.println (path);
//  int length = fileLength(path);
//  String absentIDs = "";
//  String contents = sd.getContent (path);
//  Serial.print ("Line: ");
//  Serial.println (contents);
//  
//  int commaCount = countChars (dataIDs, ',');
//  for (int c = 0; c < commaCount; c ++) {
//    String line = getValue (dataIDs, ',', c);
//
//    
//  }
//  for (int c = 1; c < length; c++) {
//    String content = getValue(contents, '\n', c);
//    Serial.print ("Line: ");
//    Serial.println (content);
//    if (countChars(content, ',') < 2) {
//      String id = getValue(content, ',', 0);
//      if (id.equals ("0"))
//        continue;
//
//      if (absentIDs != "") absentIDs += ",";
//      absentIDs += id;
//    }
//  }
//
//  return absentIDs;
//}
String  getDataId(String datapath) {
  int length  = fileLength(datapath);
  String ids = "";
  String contents = sd.getContent(datapath);
  
  for (int c = 1; c < length; c++) {
    String content = getValue(contents, '\n', c);
    String idstr = getValue(content, ',', 1);
    
    if (idstr.equals("0"))
      continue;
    // 
    if (ids != "") ids += ",";
    ids += idstr;
  }
  return ids;
}

String  getLogId (String logPath) {
  int length  = fileLength(logPath);
  
  String ids = "";
  String contents = sd.getContent(logPath);
  
  for (int c = 1; c < length; c++) {
    String content = getValue(contents, '\n', c);
    String idstr = getValue(content, ',', 0);
    
    if (idstr.equals("0"))
      continue;

    idstr.trim ();
    if (ids != "" && idstr.length () != 0) ids += ",";
    ids += idstr;
  }
  return ids;
}

void getAb(String dataPath, String path, int id) {
  boolean en = false;
  // int differ = fileLength(dataPath) - fileLength(path);
  if (id == 0)
    return;
  for (int c = 1; c < fileLength(dataPath); c++) {
    if (id == c) {
      en = true;
      if (!en)
        return id;
    }
  }
  return id;
}
