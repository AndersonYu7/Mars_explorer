/*
It's for ramps 1.6 with TB6600, you can see the pins wiring in "pins_RAMPS.h"

TB6600(E2) -> S1 : on   -> step : 200
              S2 : on
              S3 : off
              S4 : on   -> 1.5A ~ 1.7A (NEMA17 : 1.7A max)
              S5 : on
              S6 : off


              NEMA17 -> black : A+
                        green : A-
                        blue  : B+
                        red   : B-

RAMPS : 
            drv8825(NEMA17) -> black : 2B
                               green : 2A
                               blue  : 1A
                               red   : 1B

            drv8825(35 stepper motor) -> red    : 2B
                                         yellow : 2A 
                                         orange : 1A
                                         brown  : 1B

            drv8825(35 stepper motor) -> black  : 2B
                                         green  : 2A
                                         red    : 1A 
                                         blue   : 1B

limit switch : no -> signal, c -> gnd

why INPUT_PULLUP : https://blog.csdn.net/xiaoxiaoxiaoali/article/details/104338747
*/
#define J1stepPin 16  //E2
#define J1dirPin 17
#define J1_ENABLE_PIN 23
#define J1limitSwitch 25

#define J2stepPin 54  //X
#define J2dirPin 55
#define J2_ENABLE_PIN 38
//#define J2limitSwitch

#define J3stepPin 60  //Y
#define J3dirPin 61
#define J3_ENABLE_PIN 56
//#define J3limitSwitch

#define J4stepPin 26  //E0
#define J4dirPin 28
#define J4_ENABLE_PIN 24

#define J5stepPin 36  //E1
#define J5dirPin 34
#define J5_ENABLE_PIN 30

#define J6stepPin 46  //Z
#define J6dirPin 48
#define J6_ENABLE_PIN 62

#define stepPerRevolution 400
void setup() {
  // put your setup code here, to run once:
  pinMode(J1stepPin, OUTPUT);
  pinMode(J1dirPin, OUTPUT);
  pinMode(J1_ENABLE_PIN, OUTPUT);
  pinMode(J1limitSwitch, INPUT_PULLUP);
  digitalWrite(J1_ENABLE_PIN, LOW);

  pinMode(J2stepPin, OUTPUT);
  pinMode(J2dirPin, OUTPUT);
  pinMode(J2_ENABLE_PIN, OUTPUT);
  //pinMode(J2limitSwitch, INPUT_PULLUP);
  digitalWrite(J2_ENABLE_PIN, LOW);

  pinMode(J3stepPin, OUTPUT);
  pinMode(J3dirPin, OUTPUT);
  pinMode(J3_ENABLE_PIN, OUTPUT);
  //pinMode(J3limitSwitch, INPUT_PULLUP);
  digitalWrite(J3_ENABLE_PIN, LOW);

  pinMode(J4stepPin, OUTPUT);
  pinMode(J4dirPin, OUTPUT);
  pinMode(J4_ENABLE_PIN, OUTPUT);
  digitalWrite(J4_ENABLE_PIN, LOW);

  pinMode(J5stepPin, OUTPUT);
  pinMode(J5dirPin, OUTPUT);
  pinMode(J5_ENABLE_PIN, OUTPUT);
  digitalWrite(J5_ENABLE_PIN, LOW);

  pinMode(J6stepPin, OUTPUT);
  pinMode(J6dirPin, OUTPUT);
  pinMode(J6_ENABLE_PIN, OUTPUT);
  digitalWrite(J6_ENABLE_PIN, LOW);

  Serial.begin(115200);
}

//1 -> clockwise, 0 -> counterclockwise
void step_fun(const int step_, const int dir, int direction_)
{
  if(direction_ == 1) digitalWrite(dir, HIGH);
  else digitalWrite(dir, LOW);
  
  for(int i=0;i<stepPerRevolution; i++){
    
    digitalWrite(step_, HIGH);
    delayMicroseconds(1000);
    digitalWrite(step_, LOW);
    delayMicroseconds(1000);
  }
  //delay(1000);
}


//1 -> clockwise, 0 -> counterclockwise
void step_fun_with_limit_switch(const int step_, const int dir, int direction_, const int limitswitch)
{
  if(direction_ == 1) digitalWrite(dir, HIGH);
  else digitalWrite(dir, LOW);
  
  for(int i=0;i<stepPerRevolution; i++){
    if(!digitalRead(limitswitch)) break;
    
    digitalWrite(step_, HIGH);
    delayMicroseconds(1000);
    digitalWrite(step_, LOW);
    delayMicroseconds(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  step_fun(J2stepPin, J2dirPin, 1);
  step_fun(J3stepPin, J3dirPin, 1);
  step_fun(J4stepPin, J4dirPin, 1);

  while(1) step_fun_with_limit_switch(J1stepPin, J1dirPin, 1, J1limitSwitch);
}
