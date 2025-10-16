import processing.net.*;

Client esp;
String data = "";

void setup() {
  size(400, 200);
  println("Connecting...");

  esp = new Client(this, "192.168.4.1", 5000);
  if (esp.active()) {
    println("Connected");
  } else {
    println("Failed to connect.");
  }
}

void draw() {
  if (esp != null && esp.available() > 0) {
    String msg = esp.readStringUntil('\n');
    if (msg != null) {
      msg = msg.trim();
      if (msg.length() > 0) {
        println("payload" + msg);
      }
    }
  }
}
