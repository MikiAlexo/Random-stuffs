import processing.serial.*;
import static javax.swing.JOptionPane.*;

int size = 50;
PImage simage;
PImage photo;
PImage logo;
PImage img;
PImage imgs;
PImage backimg;
PImage salaryLo;
PImage settingLo;
PImage deleteLo;
PImage registerLo;
String PATH="";
String page="app";
String monthsOfTheYear [] = {"Jan", "Feb", "Mar", "April", "May", "Jun", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
String date =  monthsOfTheYear [month() - 1]+"/"+year();
String indexPath;
boolean back = false;
boolean expanded = false;

Serial port;
TextField fullName, ID, salary, NAME, Id, Linhour, Linminute, Louthour, Loutminute, Shour, Sminute, Smonth, Syear, Sday;

void setup() {
  port=new Serial(this,"COM18",9600);
  size(1366, 768);
  surface.setResizable(true);
  //registerLo=loadImage("C:/Users/Lenovo/Downloads/New folder/reg.jpg");
  //deleteLo=loadImage("C:/Users/Lenovo/Downloads/New folder/del.jpg");
  //settingLo=loadImage("C:/Users/Lenovo/Downloads/New folder/simage2.png");
  ////salaryLo=loadImage("C:/Users/Tatek/Downloads/salaryLogo.png");
  //backimg=loadImage("C:/Users/Lenovo/Downloads/New folder/back2.png");
  //simage= loadImage("C:/Users/Lenovo/Downloads/New folder/simage2.png");
  //photo = loadImage("C:/Users/Lenovo/Downloads/New folder/blue background list.jpg");
  //img = loadImage("C:/Users/Lenovo/Downloads/New folder/ui2.jpg");
  //imgs =loadImage("C:/Users/Lenovo/Downloads/New folder/logo2-removebg-preview.png");
  //logo = loadImage("C:/Users/Lenovo/Downloads/New folder/fingerimage.jpg");
  registerLo=loadImage(dataPath ("") + "/images/reg.jpg");
  deleteLo=loadImage(dataPath ("") + "/images/del.jpg");
  settingLo=loadImage(dataPath ("") + "/images/simage2.png");
  backimg=loadImage(dataPath ("") + "/images/back2.png");
  simage=loadImage(dataPath ("") + "/images/simage2.png");
  photo=loadImage(dataPath ("") + "/images/blue background list.jpg");
  img=loadImage(dataPath ("") + "/images/ui2.jpg");
  imgs=loadImage(dataPath ("") + "/images/logo2-removebg-preview.png");
  logo = loadImage(dataPath ("") + "/images/fingerimage.jpg");
  surface.setIcon(logo);
  surface.setResizable(true);
  //x,y,w,h;
  fullName = new TextField ("Full Name", 150, 150, 1100, 80);
  ID = new TextField ("ID", 150, 265, 300, 80);
  ID.acceptNumbersOnly (3);
  //salary = new TextField ("salary", 800, 265, 450, 80);
  //salary.acceptNumbersOnly();
  NAME = new TextField ("Full Name", 150, 150, 500, 80);
  Id = new TextField ("ID", 700, 150, 150, 80);
  Id.acceptNumbersOnly (3);

  Shour=new TextField ("Hour", 200, 180, 150, 70);
  Shour.acceptNumbersOnly (2);
  Linhour=new TextField ("IN-Hour", 300, 380, 150, 70);
  Linhour.acceptNumbersOnly (2);
  Linminute=new TextField ("IN-Hour", 300, 490, 150, 70);
  Linminute.acceptNumbersOnly (2);
  Loutminute=new TextField ("OUT-Hour", 600, 490, 150, 70);
  Loutminute.acceptNumbersOnly (2);
  Louthour=new TextField ("OUT-Hour", 600, 380, 150, 70);
  Louthour.acceptNumbersOnly (2);
  Sminute=new TextField ("Minute", 400, 180, 150, 70);
  Sminute.acceptNumbersOnly (2);
  Smonth = new TextField ("Month", 800, 180, 150, 70);
  Smonth.acceptNumbersOnly (2);
  Sday = new TextField ("Day", 600, 180, 150, 70);
  Sday.acceptNumbersOnly(2);
  Syear = new TextField ("Year", 1000, 180, 150, 70);
  Syear.acceptNumbersOnly(4);

  indexPath = dataPath ("") + "/project web/index.html";
}
void draw() {

  background (0);
  //println(page);
  //println(mouseX,mouseY);
  if (page=="app") 
    firstUI();
  else if (page=="reg")
    Register();
  else if (page=="del")
    delete();
  else if (page=="set")
    setting();
  if (expanded) {
    expandedNavigator("Setting");
    image(settingLo, 485, 6, 70, 50);
    if (page=="reg") {
      expandedNavigator("Delete");
      image(deleteLo, 250, 10, 40, 40);
    } else if (page=="del") {
      expandedNavigator("Register");
      image(registerLo, 250, 10, 40, 40);
    }
  }
}
void expandedNavigator (String page) {
  textAlign(LEFT, CENTER);
  fill(#D0F0EF);
  rect(100, 5, 200, 50, 40);
  fill(0);
  textSize(30);
  text(page, 130, 25);
  if (mouseX>350&&mouseX<350+250&&mouseY>15&&mouseY<15+30) {
    fill(#417BBF);
  } 
  fill(#0F54D8);
  textSize(20);
  text("User manual", 565, 30);
  if (page=="Setting") {
    textAlign(LEFT, CENTER);
    fill(#D0F0EF);
    rect(350, 5, 200, 50, 40);
    fill(0);
    textSize(30);
    text(page, 380, 25);
  }
}
