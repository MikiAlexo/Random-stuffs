// loading page
void firstUI() {
  image(photo, CENTER, CENTER, 1400, 800);
  fill (#2CB0FF);
  textSize(50);
  textAlign(CENTER, CENTER );
  text ("BIOMETRIC ATENDANCE", 400, 70);
  image(imgs, 683, 384, 140, 140);
  stroke(#2CB0FF);
  strokeWeight(1);
  line(105, 110, 1250, 110);  
  fill(255);
  textSize(30);
  textAlign(CENTER, CENTER);
  text( "loading....             ", 580, 450);
  fill(#2CB0FF);

  text(date, 1100, 90);
  if (millis() >5000)
    page="reg";
}
// registeration page
void Register() {
  image(photo, 0, 0, 1400, 800);
  textSize(40);
  fill(#8B8C93);
  textAlign(CENTER, CENTER);
  text("REGISTERATION FORM", 400, 80);
  stroke(#89F061);
  line(150, 110, 1250, 110);
  stroke(0);
  if (mouseX>550&&mouseX<550+150&&mouseY>350&&mouseY<350+80) {
    fill(#4F5DD8);
  } else
    fill(#9DDEDA);
  rect(550, 350, 150, 50, 15);
  fill(0);
  textSize(30);
  textAlign(CENTER, CENTER);
  text("SAVE", 520, 330, 200, 80);
  fill(255);
  strokeWeight(0);
  rect(5, 5, 50, 30);
  strokeWeight(3);
  line(10, 10, 50, 10);
  line(10, 20, 50, 20);
  line(10, 30, 50, 30);
  fill(0);
  fullName.draw ();
  ID.draw ();
  //salary.draw();
}

//delete page
void delete() {
  image(photo, 0, 0, 1400, 800);
  textSize(40);
  fill(#8B8C93);
  textAlign(CENTER, CENTER);
  text("DELETE REGISTEREE FORM", 750, 90);
  stroke(#89F061);
  line(150, 110, 1250, 110);
  stroke(0);
  textAlign(CENTER, CENTER);
  if (mouseX>950&&mouseX<950+150&&mouseY>165&&mouseY<165+50) {
    fill(#4F5DD8);
  } else
    fill(#9DDEDA);
  rect(950, 165, 150, 50, 15);
  fill(0);
  textSize(30);
  textAlign(CENTER, CENTER);
  text("Delete", 920, 150, 200, 80);
  fill(255);
  strokeWeight(0);
  rect(5, 5, 50, 30);
  strokeWeight(3);
  line(10, 10, 50, 10);
  line(10, 20, 50, 20);
  line(10, 30, 50, 30);
  NAME.draw ();
  Id.draw ();
}
void setting() {
  fill(255);
  strokeWeight(0);
  rect(5, 5, 50, 30);
  strokeWeight(3);
  line(10, 10, 50, 10);
  line(10, 20, 50, 20);
  line(10, 30, 50, 30);
  image(photo, 0, 0, 1400, 800);
  stroke(#2CB0FF);
  strokeWeight(1);
  line(105, 110, 1250, 110);
  textSize(40);
  fill(#8B8C93);
  textAlign(CENTER, CENTER);
  text("Settings", 230, 80);
  textSize(20);
  text("Set time:", 150, 190);
  text("set time limit:", 150, 390);
  if (mouseX>1200&&mouseX<1300&&mouseY>200&&mouseY<240) {
    fill(#4F5DD8);
  } else
    fill(#D0F0EF);  
  stroke(#8B8C93);
  rect(1200, 200, 100, 40);
  if (mouseX>1100&&mouseX<1200&&mouseY>500&&mouseY<540) {
    fill(#4F5DD8);
  } else
    fill(#D0F0EF);
  rect(1100, 500, 100, 40);
  fill(0);
  text("set", 1250, 215);
  text("set", 1150, 515);
  Shour.draw();
  Sminute.draw();
  Smonth.draw();
  Syear.draw();
  Sday.draw();
  Linminute.draw();
  Linhour.draw();
  Louthour.draw();
  Loutminute.draw();
}
