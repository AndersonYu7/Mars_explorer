#include <math.h>

#define J1stepPin 54      //x
#define J1dirPin 55
#define J1_ENABLE_PIN 38
#define J1limitSwitch 32

#define J2stepPin 60      //y
#define J2dirPin 61
#define J2_ENABLE_PIN 56
#define J2limitSwitch 33

#define J3stepPin 46      //z
#define J3dirPin 48
#define J3_ENABLE_PIN 62
#define J3limitSwitch 34


#define J4stepPin 26      //E0
#define J4dirPin 28
#define J4_ENABLE_PIN 24

#define J5stepPin 36      //E1
#define J5dirPin 34
#define J5_ENABLE_PIN 30

int ind1;              
int ind2;
int ind3;
int ind4;
int ind5;
int ind6;
int ind7;
int ind8;
int ind9;
int ind10;
int ind11;
int ind12;
int ind13;
int ind14;

int ind21;
int ind22;
int ind23;
int ind24;
int ind25;
int ind26;

int indJ1;

float J1newSteps = 0;
float J1prevSteps = 0;
float J1stepsToGo = 0;
int J1currentSteps = 0;
int J1dir;
int J1minSteps = -625;       
int J1maxSteps = 625;
bool J1jogging;


float J2newSteps = 0;
float J2prevSteps = 0;
float J2stepsToGo = 0;
int J2currentSteps = 0;
int J2dir;
int J2minSteps = 0;       
int J2maxSteps = 500;
bool J2jogging;


float J3newSteps = 0;
float J3prevSteps = 0;
float J3stepsToGo = 0;
int J3currentSteps = 0;
int J3dir;
int J3minSteps = -382;       
int J3maxSteps = 243;
bool J3jogging;


float J4newSteps = 0;
float J4prevSteps = 0;
float J4stepsToGo = 0;
int J4currentSteps = 0;
int J4dir;
int J4minSteps = -375;       
int J4maxSteps = 375;
bool J4jogging;


float J5newSteps = 0;
float J5prevSteps = 0;
float J5stepsToGo = 0;
int J5currentSteps = 0;
int J5dir;
int J5minSteps = -625;       
int J5maxSteps = 625;
bool J5jogging;

bool move1 = false;
float J1WP1;
float J2WP1;
float J3WP1;
float J4WP1;
float J5WP1;

bool move2 = false;
float J1WP2;
float J2WP2;
float J3WP2;
float J4WP2;
float J5WP2;

bool move3 = false;
float J1WP3;
float J2WP3;
float J3WP3;
float J4WP3;
float J5WP3;

long commonCurrentStep = 0;
long highestStepInt = 0;
bool driveToAngle = false;
bool driveProgram = false;

void setup()
{
  // put your setup code here, to run once:
  pinMode(J1stepPin,OUTPUT);
  pinMode(J1dirPin,OUTPUT);
  pinMode(J1_ENABLE_PIN, OUTPUT);
  pinMode(J1limitSwitch, INPUT_PULLUP);
  digitalWrite(J1dirPin, HIGH);
  digitalWrite(J1_ENABLE_PIN, HIGH);
  
  pinMode(J2stepPin,OUTPUT);
  pinMode(J2dirPin,OUTPUT);
  pinMode(J2_ENABLE_PIN, OUTPUT);
  //pinMode(J2limitSwitch, INPUT);
  digitalWrite(J2dirPin, HIGH);
  digitalWrite(J2_ENABLE_PIN, HIGH);

  pinMode(J3stepPin,OUTPUT);
  pinMode(J3dirPin,OUTPUT);
  pinMode(J3_ENABLE_PIN, OUTPUT);
  //pinMode(J3limitSwitch, INPUT);
  digitalWrite(J3dirPin, HIGH);
  digitalWrite(J3_ENABLE_PIN, HIGH);

  pinMode(J4stepPin,OUTPUT);
  pinMode(J4dirPin,OUTPUT);
  pinMode(J4_ENABLE_PIN, OUTPUT);
  digitalWrite(J4dirPin, HIGH);
  digitalWrite(J4_ENABLE_PIN, HIGH);

  pinMode(J5stepPin,OUTPUT);
  pinMode(J5dirPin,OUTPUT);
  pinMode(J5_ENABLE_PIN, OUTPUT);
  digitalWrite(J5dirPin, HIGH);
  digitalWrite(J5_ENABLE_PIN, HIGH);

  
  J1jogging = false;
  J2jogging = false;
  J3jogging = false;
  J4jogging = false;
  J5jogging = false;
  
  Serial.begin(115200);
}

unsigned long time_since_last_reset = 0;
unsigned long time1;
unsigned long time2;
unsigned long time3;
unsigned long time4;
unsigned long time5;


void drive(String data)//control all motors simultaneously, and start and end at the same time
{
  if(data.startsWith("drive"))
  {
    J1jogging = false;
    J2jogging = false;
    J3jogging = false;
    J4jogging = false;
    J5jogging = false;
    
    //driving
    Serial.println(data);
    ind1 = data.indexOf(',');
    ind2 = data.indexOf(',', ind1+1);
    J1newSteps = data.substring(ind1+1, ind2).toFloat();
    ind3 = data.indexOf(',', ind2+1);
    J2newSteps = data.substring(ind2+1, ind3).toFloat();
    ind4 = data.indexOf(',', ind3+1);
    J3newSteps = data.substring(ind3+1, ind4).toFloat();
    ind5 = data.indexOf(',', ind4+1);
    J4newSteps = data.substring(ind4+1, ind5).toFloat();
    J5newSteps = data.substring(ind5+1).toFloat();

    Serial.print("J1new:");
    Serial.println(J1newSteps);
    Serial.print("J2new:");
    Serial.println(J2newSteps);
    Serial.print("J3new:");
    Serial.println(J3newSteps);
    Serial.print("J4new:");
    Serial.println(J4newSteps);
    Serial.print("J5new:");
    Serial.println(J5newSteps);
    data = "";
    driveToAngle = true;
  }
  
  if(driveToAngle == true)
  {
    //calculate directions and step amounts
    if (J1newSteps < J1prevSteps){
      J1dir = 0;
      J1stepsToGo = J1prevSteps - J1newSteps;
    }
    if (J1newSteps >= J1prevSteps){
      J1dir = 1;
      J1stepsToGo = J1newSteps - J1prevSteps;
    }
    
    if (J2newSteps < J2prevSteps){
      J2dir = 0;
      J2stepsToGo = J2prevSteps - J2newSteps;
    }
    if (J2newSteps >= J2prevSteps){
      J2dir = 1;
      J2stepsToGo = J2newSteps - J2prevSteps;
    }
    if (J3newSteps < J3prevSteps){
      J3dir = 0;
      J3stepsToGo = J3prevSteps - J3newSteps;
    }
    if (J3newSteps >= J3prevSteps){
      J3dir = 1;
      J3stepsToGo = J3newSteps - J3prevSteps;
    }

    if (J4newSteps < J4prevSteps){
      J4dir = 0;
      J4stepsToGo = J4prevSteps - J4newSteps;
    }
    if (J4newSteps >= J4prevSteps){
      J4dir = 1;
      J4stepsToGo = J4newSteps - J4prevSteps;
    }

    if (J5newSteps < J5prevSteps){
      J5dir = 0;
      J5stepsToGo = J5prevSteps - J5newSteps;
    }
    if (J5newSteps >= J5prevSteps){
      J5dir = 1;
      J5stepsToGo = J5newSteps - J5prevSteps;
    }
    
    float highestStep = J1stepsToGo; 
    if(J2stepsToGo > highestStep){
      highestStep = J2stepsToGo;
    }
         
    if(J3stepsToGo > highestStep){
      highestStep = J3stepsToGo;
    }
    
    if(J4stepsToGo > highestStep){
      highestStep = J4stepsToGo;
    }

    if(J5stepsToGo > highestStep){
      highestStep = J5stepsToGo;
    }

    float resolutionMultiplier = 10; //maybe motor number * 2

    float J1stepSkipF = highestStep / J1stepsToGo * resolutionMultiplier;       //Finding the numbers of steps that each stepper has to skip in order for the moves to be started and stopped simultaneously
    float J2stepSkipF = highestStep / J2stepsToGo * resolutionMultiplier;       //for example stepper 1 is taking a step every J1stepSkip step. IE if J1stepSkip is 4, then stepper 1 takes a step every fourth program loop
    float J3stepSkipF = highestStep / J3stepsToGo * resolutionMultiplier;
    float J4stepSkipF = highestStep / J4stepsToGo * resolutionMultiplier;
    float J5stepSkipF = highestStep / J5stepsToGo * resolutionMultiplier;

    int J1stepSkip = (int)J1stepSkipF;
    int J2stepSkip = (int)J2stepSkipF;
    int J3stepSkip = (int)J3stepSkipF;
    int J4stepSkip = (int)J4stepSkipF;
    int J5stepSkip = (int)J5stepSkipF;

    digitalWrite(J1dirPin, J1dir);
    digitalWrite(J2dirPin, J2dir);
    digitalWrite(J3dirPin, J3dir);
    digitalWrite(J4dirPin, J4dir);
    digitalWrite(J5dirPin, J5dir);

    Serial.print("highestStep:");
    Serial.println(highestStep);
    if(highestStep == 0){
      highestStepInt = 0;
    }
    else highestStepInt = (long)highestStep * resolutionMultiplier;

  int current_time = 600;    

  float x = 2400;
  float y;
  
  bool Jxfinish = false;

  //Serial.println(current_time);

  while(commonCurrentStep < highestStep){
    if(commonCurrentStep < J1stepsToGo){
      digitalWrite(J1stepPin,HIGH);
      delayMicroseconds(current_time);
      digitalWrite(J1stepPin, LOW);
    }

    if(commonCurrentStep < J2stepsToGo){
      digitalWrite(J2stepPin,HIGH);
      delayMicroseconds(current_time);
      digitalWrite(J2stepPin, LOW);
    }

    if(commonCurrentStep < J3stepsToGo){
      digitalWrite(J3stepPin,HIGH);
      delayMicroseconds(current_time);
      digitalWrite(J3stepPin, LOW);
    }

    if(commonCurrentStep < J4stepsToGo){
      digitalWrite(J4stepPin,HIGH);
      delayMicroseconds(current_time);
      digitalWrite(J4stepPin, LOW);
    }

    if(commonCurrentStep < J5stepsToGo){
      digitalWrite(J5stepPin,HIGH);
      delayMicroseconds(current_time);
      digitalWrite(J5stepPin, LOW);
    }

    commonCurrentStep++;
    if(commonCurrentStep == J1stepsToGo || commonCurrentStep == J2stepsToGo || commonCurrentStep == J3stepsToGo || commonCurrentStep == J4stepsToGo || commonCurrentStep == J5stepsToGo)
      Jxfinish = true;      

    if(Jxfinish){
      y = 600*(600*5/x) + 100;
      current_time = y;      
      x = x - 600;
    }
    
 }
  //Serial.println(current_time);
  
   
    J1stepsToGo = 0;
    J1currentSteps = 0;
    J1prevSteps = J1newSteps;
    J2stepsToGo = 0;
    J2currentSteps = 0;
    J2prevSteps = J2newSteps;
    J3stepsToGo = 0;
    J3currentSteps = 0;
    J3prevSteps = J3newSteps;
    J4stepsToGo = 0;
    J4currentSteps = 0;
    J4prevSteps = J4newSteps;
    J5stepsToGo = 0;
    J5currentSteps = 0;
    J5prevSteps = J5newSteps;
    commonCurrentStep = 0;
    highestStepInt = 0;

    driveToAngle = false;
  }
}

void jogging(String data) //contorl simple motor
{
  if(data.startsWith("jogJ1-")){
    J1jogging = true;
    J1dir = 0; //LOW -> counterclockwise
    digitalWrite(J1dirPin, J1dir);
    driveToAngle = false;
  }
  else if(data.startsWith("jogJ1+")){
    J1jogging = true;
    J1dir = 1; //HIGH -> clockwise
    digitalWrite(J1dirPin, J1dir);
    driveToAngle = false;
  }

  if(data.startsWith("jogJ2-")){
    J2jogging = true;
    J2dir = 0; //LOW -> counterclockwise
    digitalWrite(J2dirPin, J2dir);
    driveToAngle = false;
  }
  else if(data.startsWith("jogJ2+")){
    J2jogging = true;
    J2dir = 1; //HIGH -> clockwise
    digitalWrite(J2dirPin, J2dir);
    driveToAngle = false;
  }
  if(data.startsWith("jogJ3-")){
    J3jogging = true;
    J3dir = 0; //LOW -> counterclockwise
    digitalWrite(J3dirPin, J3dir);
    driveToAngle = false;
  }
  else if(data.startsWith("jogJ3+")){
    J3jogging = true;
    J3dir = 1; //HIGH -> clockwise
    digitalWrite(J3dirPin, J3dir);
    driveToAngle = false;
  }

  if(data.startsWith("jogJ4-")){
    J4jogging = true;
    J4dir = 0; //LOW -> counterclockwise
    digitalWrite(J4dirPin, J4dir);
    driveToAngle = false;
  }
  else if(data.startsWith("jogJ4+")){
    J4jogging = true;
    J4dir = 1; //HIGH -> clockwise
    digitalWrite(J4dirPin, J4dir);
    driveToAngle = false;
  }

  if(data.startsWith("jogJ5-")){
    J5jogging = true;
    J5dir = 0; //LOW -> counterclockwise
    digitalWrite(J5dirPin, J5dir);
    driveToAngle = false;
  }
  else if(data.startsWith("jogJ5+")){
    J5jogging = true;
    J5dir = 1; //HIGH -> clockwise
    digitalWrite(J5dirPin, J5dir);
    driveToAngle = false;
  }

  if(J1jogging == true)
  {
      if(J1dir == 1 && J1prevSteps < J1maxSteps){ //clockwise
        J1prevSteps++;

        digitalWrite(J1stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J1stepPin, LOW);
      }
      if(J1dir == 0 && J1minSteps < J1prevSteps){ //counterclockwise
        J1prevSteps--;

        digitalWrite(J1stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J1stepPin, LOW);
      }
  }
  
  if(J2jogging == true)
  {
      if(J2dir == 1 && J2prevSteps < J2maxSteps){ //clockwise
        J2prevSteps++;

        digitalWrite(J2stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J2stepPin, LOW);
      }
      if(J2dir == 0 && J2minSteps < J2prevSteps){ //counterclockwise
        J2prevSteps--;

        digitalWrite(J2stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J2stepPin, LOW);
      }
  }

  if(J3jogging == true)
  {
      if(J3dir == 1 && J3prevSteps < J3maxSteps){ //clockwise
        J3prevSteps++;

        digitalWrite(J3stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J3stepPin, LOW);
      }
      if(J3dir == 0 && J3minSteps < J3prevSteps){ //counterclockwise
        J3prevSteps--;

        digitalWrite(J3stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J3stepPin, LOW);
      }
  }

  if(J4jogging == true)
  {
      if(J4dir == 1 && J4prevSteps < J4maxSteps){ //clockwise
        J4prevSteps++;

        digitalWrite(J4stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J4stepPin, LOW);
      }
      if(J4dir == 0 && J4minSteps < J4prevSteps){ //counterclockwise
        J4prevSteps--;

        digitalWrite(J4stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J4stepPin, LOW);
      }
  }

  if(J5jogging == true)
  {
      if(J5dir == 1 && J5prevSteps < J5maxSteps){ //clockwise
        J5prevSteps++;

        digitalWrite(J5stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J5stepPin, LOW);
      }
      if(J5dir == 0 && J5minSteps < J5prevSteps){ //counterclockwise
        J5prevSteps--;

        digitalWrite(J5stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J5stepPin, LOW);
      }
  }
}

void enable_disable(String data)
{
    if(data == "J1enable")
  {
    digitalWrite(J1_ENABLE_PIN, LOW);
  }
  if(data == "J1disable")
  {
    digitalWrite(J1_ENABLE_PIN, HIGH);
  }
  if(data == "J2enable")
  {
    digitalWrite(J2_ENABLE_PIN, LOW);
  }
  if(data == "J2disable")
  {
    digitalWrite(J2_ENABLE_PIN, HIGH);
  }
  if(data == "J3enable")
  {
    digitalWrite(J3_ENABLE_PIN, LOW);
  }
  if(data == "J3disable")
  {
    digitalWrite(J3_ENABLE_PIN, HIGH);
  }
  if(data == "J4enable")
  {
    digitalWrite(J4_ENABLE_PIN, LOW);
  }
  if(data == "J4disable")
  {
    digitalWrite(J4_ENABLE_PIN, HIGH);
  }
  if(data == "J5enable")
  {
    digitalWrite(J5_ENABLE_PIN, LOW);
  }
  if(data == "J5disable")
  {
    digitalWrite(J5_ENABLE_PIN, HIGH);
  }

  if(data == "enableAll")
  {
    digitalWrite(J1_ENABLE_PIN, LOW);
    digitalWrite(J2_ENABLE_PIN, LOW);
    digitalWrite(J3_ENABLE_PIN, LOW);
    digitalWrite(J4_ENABLE_PIN, LOW);
    digitalWrite(J5_ENABLE_PIN, LOW);
  }
  if(data == "disableAll")
  {
    digitalWrite(J1_ENABLE_PIN, HIGH);
    digitalWrite(J2_ENABLE_PIN, HIGH);
    digitalWrite(J3_ENABLE_PIN, HIGH);
    digitalWrite(J4_ENABLE_PIN, HIGH);
    digitalWrite(J5_ENABLE_PIN, HIGH);
  }
}

void move_(String data)
{
  if(data == "saveWP1"){
    J1WP1 = J1prevSteps;
    J2WP1 = J2prevSteps;
    J3WP1 = J3prevSteps;
    J4WP1 = J4prevSteps;
    J5WP1 = J5prevSteps;
  
    move1 = true;
  }

  if(data == "saveWP2"){
    J1WP2 = J1prevSteps;
    J2WP2 = J2prevSteps;
    J3WP2 = J3prevSteps;
    J4WP2 = J4prevSteps;
    J5WP2 = J5prevSteps;
  
    move2 = true;
  }

  if(data == "saveWP3"){
    J1WP3 = J1prevSteps;
    J2WP3 = J2prevSteps;
    J3WP3 = J3prevSteps;
    J4WP3 = J4prevSteps;
    J5WP3 = J5prevSteps;
  
    move3 = true;
  }

  if(data == "PLAY"){
    driveProgram = true;
  }

  if(driveProgram == true){
    if(move1 == true){
      J1newSteps = J1WP1;
      J2newSteps = J2WP1;
      J3newSteps = J3WP1;
      J4newSteps = J4WP1;
      J5newSteps = J5WP1;
  
      driveToAngle = true;
    }
    
    if(move2 == true){
      J1newSteps = J1WP2;
      J2newSteps = J2WP2;
      J3newSteps = J3WP2;
      J4newSteps = J4WP2;
      J5newSteps = J5WP2;
  
      driveToAngle = true;
    }
  
    if(move3 == true){
      J1newSteps = J1WP3;
      J2newSteps = J2WP3;
      J3newSteps = J3WP3;
      J4newSteps = J4WP3;
      J5newSteps = J5WP3;
  
      driveToAngle = true;
    }
  }
}

void inital(String data)
{
  bool inital_ = false;
  if(data == "inital") inital_ = true;
  
    if(inital_==true){
      //J1
      digitalWrite(J1dirPin, LOW); //counterclockwise
      
      for(int i=0;i<625*4; i++){
        if(!digitalRead(J1limitSwitch)) break;          
 
        digitalWrite(J1stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(J1stepPin, LOW);
        delayMicroseconds(1000);
      }
      delay(1000);
      digitalWrite(J1dirPin, HIGH); //clockwise
      for(int i=0;i<625; i++){
 
        digitalWrite(J1stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(J1stepPin, LOW);
        delayMicroseconds(1000);
      }
      J1prevSteps = 0;

      //J2
      digitalWrite(J2dirPin, LOW); //counterclockwise
      
      for(int i=0;i<500; i++){
        if(!digitalRead(J2limitSwitch)) break;          
 
        digitalWrite(J2stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(J2stepPin, LOW);
        delayMicroseconds(1000);
      }
      delay(1000);
      J2prevSteps = 0;  

      //J3
      digitalWrite(J1dirPin, HIGH); //clockwise
      
      for(int i=0;i<625; i++){
        if(!digitalRead(J3limitSwitch)) break;          
 
        digitalWrite(J3stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(J3stepPin, LOW);
        delayMicroseconds(1000);
      }
      delay(1000);
      digitalWrite(J3dirPin, LOW); //counterclockwise
      for(int i=0;i<382; i++){
 
        digitalWrite(J3stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(J3stepPin, LOW);
        delayMicroseconds(1000);
      }
      J3prevSteps = 0;            
    }
}

void loop() {
  // put your main code here, to run repeatedly:
 
  String readString;
  String data;
  char c;

  while(Serial.available()){
    delay(1);
    if(Serial.available()>0){
      c = Serial.read(); //enter is the control char
      if(isControl(c)) break;

      readString += c;
    }
  }
  data = readString;
  
  drive(data);
  jogging(data);
  enable_disable(data);
  move_(data);
  inital(data);
}
