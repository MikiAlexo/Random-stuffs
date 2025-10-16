// mouse & keyboard input
void mousePressed () {
  fullName.mousePressed();
  ID.mousePressed();
  //salary.mousePressed();
  NAME.mousePressed();
  Id.mousePressed();
  Linhour.mousePressed();
  Linminute.mousePressed();
  Louthour.mousePressed();
  Loutminute.mousePressed();
  Shour.mousePressed();
  Sminute.mousePressed();
  Smonth.mousePressed();
  Syear.mousePressed();
  Sday.mousePressed();
}
void keyPressed () {
  fullName.keyPressed();
  ID.keyPressed();
  Linhour.keyPressed();
  Linminute.keyPressed();
  Louthour.keyPressed();
  Loutminute.keyPressed();
  Shour.keyPressed();
  Sminute.keyPressed();
  Smonth.keyPressed();
  Syear.keyPressed();
  Sday.keyPressed();

  // salary.keyPressed();
  NAME.keyPressed();
  Id.keyPressed();

  if (int(ID.value)>255) {
    ID.showErrorText("Id should be less than 255");
  }
  if (int(Linhour.value)>255) {
    ID.showErrorText("Id should be less than 255");
  } else
    ID.error = false;

  if (int(Id.value)>255) {
    Id.showErrorText("Id doesn't exist");
  } else
    Id.error = false;

  if (fullName.value.length()<3 && fullName.value.length()>0)
    fullName.showErrorText("name should contain at least 3 characteres");
  else
    fullName.error = false;

  if (NAME.value.length()<3 && NAME.value.length()>0)
    NAME.showErrorText("name should contain at least 3 characteres");
  else
    NAME.error = false;


  if (keyCode == ESC && page=="reg") {
    //delay(200);
    exit();
  }
  // if (keyCode == ESC && page=="del") {
  //   page="reg";
  //  println("back to register");
  //}
  // println (fullName.value);
  //println (ID.value);
  //}
}

void mouseClicked() {
  if (mouseX>550&&mouseX<550+150&&mouseY>350&&mouseY<350+50) {
    if (fullName.value.length()==0||ID.value.length()==0) { 
      showMessageDialog(null, "The requirements are not fullfilled");
      print(fullName+"abiy");
      return;
    }
 
    String content = "*" + fullName.value + "," + ID.value+"*\n";
   port.write(content);
    
    // port.write("*"+fullName+"\n");
    //println("Name"+fullName.value);
    //port.write(ID+"*"+"\n");
    //println("ID"+ID.value);
    //port.write("salary"+salary+"/n");
    //   println("salary"+salary.value);
  } else if (mouseX>950&&mouseX<950+150&&mouseY>165&&mouseY<165+50) {
    if (NAME.value.length()==0||Id.value.length()==0) { 
      showMessageDialog(null, "The requirements are not fullfilled");
      return;
    }
    println("deletedName"+NAME.value);
    //port.write("4365"+Id.value+"\n");
    println("deletedId"+Id.value);
  } else if (mouseX>100&&mouseX<100+200&&mouseY>5&&mouseY<5+50&&page=="reg") {
    page="del";
  } else if (mouseX>100&&mouseX<100+200&&mouseY>5&&mouseY<5+50&&page=="del") {
    page="reg";
  } else if (mouseX>565 &&mouseX<700 && mouseY>20 && mouseY<45) {
    println (indexPath);
    File f = new File (indexPath);
    println (f.exists ());
    launch (indexPath);
  } else if (mouseX>350 &&mouseX<750 && mouseY>20 && mouseY<45)
    page="set";

  if (mouseX>1200&&mouseX<1300&&mouseY>200&&mouseY<240 && page == "set") {
    String set = "!"+Shour.value+Sminute.value+Sday.value  +Smonth.value+Syear.value+"!\n";
    if (Shour.value.length()==0||Sminute.value.length()==0||Sday.value.length()==0||Smonth.value.length()==0||Syear.value.length()==0) {
      showMessageDialog(null, "The requirements are not fullfilled");
      return;
    } else
      port.write(set);
  }
  if (mouseX>1100&&mouseX<1200&&mouseY>500&&mouseY<540) {
    String limit = "%"+Linhour.value+Linminute.value+Louthour.value+Loutminute.value+"%\n";
    if (Linhour.value.length()==0||Linminute.value.length()==0||Louthour.value.length()==0||Loutminute.value.length()==0) {
      showMessageDialog(null, "The requirements are not fullfilled");
      return;
    } else
      println(limit);
  }
}
void mouseReleased() {
  if (mouseX>5&&mouseX<5+50&&mouseY>5&&mouseY<5+30 && (page=="del" || page =="reg")) {
    expanded=true;
  } else
    expanded=false;
  ;
}
