#include <Servo.h>

Servo myservo;

int x, y;
String data;
String msg;
int ind1;
int ind2;
int ind3;
int dir = 1; // 60 -> 120
          //0 : 120 -> 60

int is_done = 0;

int theta=60;
int detected = 0;

void setup() {
 Serial.begin(115200);
 Serial.setTimeout(1);

 myservo.attach(2);
}
void loop() {
  if(detected == 0){
    if(dir == 1){
      myservo.write(theta);
      delay(1000);
      theta+=5;
      //Serial.println("left");
    } else if(dir == 0){
      myservo.write(theta);
      delay(1000);
      theta-=5;
      //Serial.println("right");
    }
    if(theta == 120) dir = 0;
    else if(theta == 60) dir = 1;

    if(Serial.available()){

      detected = 1;
    } 
    
    
  
  } else{
    if(Serial.available() && !is_done){
      data = Serial.readStringUntil('\r');
      ind1 = data.indexOf(",");
      x = data.substring(0, ind1).toInt();
      y = data.substring(ind1+1).toInt();
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
    }

    if(x - 400 > 10){
      dir = 1;
    } else if(x - 400 < -10){
      dir = 0;
    } else{
      dir = 2;
      is_done = 1;
    }

    if(dir==1 && !is_done){
      myservo.write(theta);
      delay(1500);
      theta--;
      if(theta==120) dir = 0;
    } else if(dir == 0 && !is_done ){
      myservo.write(theta);
      delay(1500);
      theta++;
      if(theta == 60) dir = 1;
    } else{
      is_done = 1;
      myservo.write(theta);
      if(Serial.available()){
        data = Serial.readStringUntil('\r');
        ind1 = data.indexOf(",");
        x = data.substring(0, ind1).toInt();
        y = data.substring(ind1+1).toInt();
        Serial.print(theta);
      }
        
    }
  }

}
