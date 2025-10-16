class TextField {
  String label = "", value = ""; // Label: Name  |  Value: Input text (content)
  String assistiveText = "Assistive Text";
  String allowedInputChars = "";
  String LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  String NUMBERS = "0123456789";
  String SPACE = " ";
  String NAME_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz /.";

  float x = 480, y = 180;
  float w = 400, h = 80;

  float minTextSize = 17.5, maxTextSize = 24;

  float aiMinBorderSize = 1.5, aiMaxBorderSize = 4; // ai: Activation Indicator
  float insertionPointSize = 2;
  float cornerRadius = 6;

  int insertionPointPeriod = 500; // Blinking period of insertion point
  int inputCharLimit = -1;

  color accentColor = #0539FF;
  color unhoveredFill = barelyGray, hoveredFill = almostGray, activeFill = lightGray; // For container fill
  color unhoveredStroke = gray, hoveredStroke = superGray, activeStroke = accentColor; // For activation indicator/border & text fill
  color inactiveLabelFill = darkGray, activeLabelFill = accentColor;
  color valueFill = superGray;
  color errorFill = reddisBrown;

  boolean active = false;
  boolean showCharCount = false;
  boolean showAssistiveText = false;
  boolean error = false;
  boolean required = false;

  Fonts fonts;

  Periodically ipPeriod;

  TextField (String label, float x, float y, float w, float h) {
    this.label = label;
    this.x = x; 
    this.y = y;
    this.w = w; 
    this.h = h;



    ipPeriod = new Periodically (insertionPointPeriod);

    allowedInputChars = NAME_CHARACTERS;

    fonts = new Fonts ();
  }  

  void draw () {
    // Container
    noStroke ();
    rectMode (CORNER);
    fill (hovered ()? hoveredFill : unhoveredFill);
    if (active) fill (activeFill);
    rect (x, y, w, h, cornerRadius, cornerRadius, 0, 0);

    // Activation Indicator
    stroke (hovered ()? hoveredStroke : unhoveredStroke);
    if (active) stroke (activeStroke);
    if (error) stroke (errorFill);
    strokeWeight (active? aiMaxBorderSize : aiMinBorderSize);
    strokeCap (SQUARE);
    line (x, y + h, x + w, y + h);

    // Label text
    textFont (fonts.roboto.regular);
    float lGap = (h - maxTextSize)/2;
    float lx = x + lGap;
    float ly = y + (h - maxTextSize)/2 - textDescent ()/2;
    if (active || value.length () != 0) ly = y + minTextSize - textDescent ()/2;
    fill (active? activeLabelFill : inactiveLabelFill);
    if (error) fill (errorFill);
    textSize (active || value.length () != 0? minTextSize : maxTextSize);
    textAlign (LEFT, TOP);
    text (error? label + "*" : label, lx, ly);

    // Insertion point | L: Length | W: Width of bounding box for the insertion point
    textFont (fonts.roboto.regular);
    textSize (maxTextSize);
    float ipX = lx + insertionPointSize*0.8 + textWidth (value);
    ipX = constrain (ipX, lx + insertionPointSize*0.8, x + w - lGap);
    float ipY = y + minTextSize + 1.9*(h - minTextSize - maxTextSize)/3;
    float ipL = (textDescent () + textAscent ())*0.85;
    float ipW = w - 2*lGap - insertionPointSize*0.8;
    if (active) stroke (activeStroke);
    if (error) stroke (errorFill);
    strokeWeight (insertionPointSize);
    if (active && ipPeriod.getState())
      line (ipX, ipY, ipX, ipY + ipL);

    // Value text
    String displayableValue = value;
    if (ipX  >= x + w - lGap) { // Insertion point has reached bound limit within field
      while (textWidth (displayableValue) > ipW) displayableValue = displayableValue.substring (1, displayableValue.length ());
    }
    textFont (fonts.roboto.regular);
    textSize (maxTextSize); 
    fill (valueFill); 
    textAlign (LEFT, TOP);
    text (displayableValue, lx, ipY);

    // Assistive text
    if (showAssistiveText) {
      textFont (fonts.roboto.regular);
      float atx = lx, aty = y + h + minTextSize*0.5;
      fill (inactiveLabelFill);
      if (error) fill (errorFill);
      textSize (minTextSize);
      textAlign (LEFT, TOP);
      text ((required? "*" : "") + assistiveText, atx, aty);
    }

    // Character Counter
    if (inputCharLimit > 0 && inputCharLimit<255) {
      textFont (fonts.roboto.regular);
      float atx = x + w - lGap, aty = y + h + minTextSize*0.5;
      fill (inactiveLabelFill);
      textSize (minTextSize);
      textAlign (RIGHT, TOP);
      text (value.length () + "/" + inputCharLimit, atx, aty);
    }
  }

  boolean hovered () {
    return rectHovered (x, y, w, h, CORNER);
  }
  boolean rectHovered (float x, float y, float w, float h, float orientation) {
    return (((orientation == CORNER && mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h)
      || (orientation == CENTER && mouseX >= x - w/2 && mouseX <= x + w/2 && mouseY >= y - h/2 && mouseY <= y + h/2))? true : false);
  }

  void acceptNumbersOnly () {
    allowedInputChars = NUMBERS;
  }
  void acceptNumbersOnly (int inputCharLimit) {
    allowedInputChars = NUMBERS;

    this.inputCharLimit = inputCharLimit;
  }
  void setCharacterLimit (int inputCharLimit) {
    this.inputCharLimit = inputCharLimit;
  }

  void showAssistiveText (String assistiveText) {
    showAssistiveText = true;

    this.assistiveText = assistiveText;
  }
  void showErrorText (String assistiveText) {
    showAssistiveText = true;
    error = true;

    this.assistiveText = assistiveText;
  }
  void setRequiredText (String assistiveText) {
    this.assistiveText = assistiveText;

    required = true;
  }
  void setRequiredText (String assistiveText, boolean showAssistiveText) {
    this.assistiveText = assistiveText;

    required = true;
    this.showAssistiveText = showAssistiveText;
  }

  void mousePressed () {
    if (hovered ()) active = true;
    else active = false;
  }

  void keyPressed () {
    if (active == true) {
      if (allowedInputChars.contains (str (key)) && (inputCharLimit > 0? value.length () < inputCharLimit : true))
        value += key;
      else if (value.length () > 0 && keyCode == BACKSPACE) {
        int index = value.length () - 1;
        value = value.substring (0, index);
      } else if (keyCode == ENTER)
        active = false;
    }
  }
}
