#include <DS3232RTC.h>
class DS3232Minimal {
  public:
    String monthsOfTheYear [12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    void initialize () {
      if (!readable ()) Serial.println("Unable to sync with the RTC");
      else Serial.println("RTC has set the system time");
    }

    boolean readable () {
      setSyncProvider(RTC.get);
      return timeStatus() == timeSet;
    }

    void setTimeDate (int _hour, int _minute, int _second, int _day, int _month, int _year) {
      time_t t;
      tmElements_t tm;

      tm.Hour = _hour;
      tm.Minute = _minute;
      tm.Second = _second;
      
      tm.Year = _year - 1970;
      tm.Month = _month;
      tm.Day = _day;

      t = makeTime (tm);

      RTC.set(t);
      setTime(t);
    }


    String dateStampNumeral () {
      String dateStamp = "";
      if (readable ())
        dateStamp = String (hour ()) + "-" + String (month ()) + "-" + String (year ());
      return dateStamp;
    }
    String namedate(){
       String dateStamp = "";
      if (readable ())
        dateStamp = String (month ()) + "-" + String (year ());
      return dateStamp;

      }
    String dateStamp () {
      String dateStamp = "";
      if (readable ())
        dateStamp = monthsOfTheYear [month () - 1] + " " + String (day ()) + ", " + String (year ());
      return dateStamp;
    }

    String timeStamp () {
      String timeStamp = "";
      if (readable ())
        timeStamp = String (hour ()) + ":" + String (minute ()) + ":" + String (second ());
      return timeStamp;
    }
    String miniTimeStamp () {
      String timeS = timeStamp ();
      String miniTimeS = "";
      for (int a = 0; a < timeS.length () - 3; a ++)
        miniTimeS += timeS [a];

      return miniTimeS;
    }
};
