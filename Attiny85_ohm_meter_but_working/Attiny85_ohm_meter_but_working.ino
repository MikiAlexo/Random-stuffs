 
  #include <TinyWireM.h>
  #include <Tiny4kOLED.h>
  
  int adata = 2; // ADC2/pin3
  int sensorvalue = 0;
  float vref = 5.0; // Internal reference voltage of ATtiny85 **used vcc cause i got problem with the internal
  float vout = 0.0; // Output voltage from the voltage divider
  float Rref = 324.0; // Reference resistance in ohms 324 for mow atleast
  float R = 0.0; // Resistance to be measured
  const char *chance[4] {
    "very probable",
    "probable",
    "not ready",
    "device error: [-1]"
  };
  void setup() {
    // Setting reference voltage to the internal 1.1V
    analogReference(DEFAULT);
  
    oled.begin();
    oled.setFont(FONT8X16);
    oled.clear();
    oled.on();
  }
  
  void loop() {
    sensorvalue = analogRead(adata); 
    vout = (vref * sensorvalue) / 1023.0;
  
    
    if (vout != vref) {
      R = Rref * ((vref / vout) - 1.0);
    } else {
      R = -1;
    }
  
  
    if (R > 320 && R < 330) {
      oled.setCursor(30, 8);
      oled.clear();
      oled.print(chance[0]); 
    }
    else if (R > 305 && R < 340) {
      oled.setCursor(30, 8);
      oled.clear();
      oled.print(chance[1]);
    }
    else if (R > 370 || R < 290) {
      oled.setCursor(30, 8);
      oled.clear();
      oled.print(chance[2]);
    }
    else if (R == -1) {
      oled.setCursor(30, 8);
      oled.clear();
      oled.print(chance[3]);
    }
    oled.setCursor(5, 50);
    oled.print(R);
    oled.clear(); 
    //delay(1000);
  }
