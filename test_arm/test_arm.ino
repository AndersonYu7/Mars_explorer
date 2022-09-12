#define J2stepPin 54      //x
#define J2dirPin 55
#define J2_ENABLE_PIN 38
#define J2limitSwitch 32

#define J3stepPin 60
#define J3dirPin 61
#define J3_ENABLE_PIN 56
//#define J3limitSwitch 39

int ind1;
int ind2;

float J2newSteps = 0;
float J2prevSteps = 0;
float J2stepsToGo = 0;
int J2currentSteps = 0;
int J2dir;
int J2minSteps = -1500;       //40000 täyteen kierrokseen
int J2maxSteps = 1500;
int J2calibrationSpeed = 1000;
int J2calibrationReturnSpeed = 500;
int J2calibrationReturn = 1650;
bool J2isCalibrated;
int J2driveSpeed = 250;
bool J2jogging;
int J2calibration = 0;

float J3newSteps = 0;
float J3prevSteps = 0;
float J3stepsToGo = 0;
int J3currentSteps = 0;
int J3dir;
int J3minSteps = -1500;       //10000 täyteen kierrokseen
int J3maxSteps = 1900;
int J3calibrationSpeed = 2000;
int J3calibrationReturnSpeed = 1000;
int J3calibrationReturn = 1620;
bool J3isCalibrated;
int J3driveSpeed = 500;
bool J3jogging;
int J3calibration = 0;

bool driveProgram = false;

bool move1 = false;
float J2WP1;

bool move2 = false;
float J2WP2;

bool move3 = false;
float J2WP3;

long commonCurrentStep = 0;
long highestStepInt = 0;

long timerTime = 0;

bool driveToAngle = false;
bool jointsCalibrating = false;

long testSteps = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(J2stepPin,OUTPUT);
  pinMode(J2dirPin,OUTPUT);
  pinMode(J2_ENABLE_PIN, OUTPUT);
  pinMode(J2limitSwitch, INPUT);
  digitalWrite(J2dirPin, HIGH);
  digitalWrite(J2_ENABLE_PIN, LOW); //enable which mean can't move motor by hand

  pinMode(J3stepPin,OUTPUT);
  pinMode(J3dirPin,OUTPUT);
  pinMode(J3_ENABLE_PIN, OUTPUT);
  //pinMode(J3limitSwitch, INPUT);
  digitalWrite(J3dirPin, HIGH);
  digitalWrite(J3_ENABLE_PIN, LOW);

  J2isCalibrated = false;
  J2jogging = false;

  Serial.begin(115200);
}

void drive(String data)//control all motors simultaneously, and start and end at the same time
{
  if(data.startsWith("drive"))
  {
    J2jogging = false;
    J3jogging = false;
    //driving
    Serial.println(data);
    ind1 = data.indexOf(',');
    ind2 = data.indexOf(',', ind1+1);
    J2newSteps = data.substring(ind1+1, ind2).toFloat();
    J3newSteps = data.substring(ind2+1).toFloat();

    Serial.print("J2new:");
    Serial.println(J2newSteps);
    Serial.print("J3new:");
    Serial.println(J3newSteps);
    data = "";
    driveToAngle = true;
  }
  
  if(driveToAngle == true)
  {
    //calculate directions and step amounts
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
    
    float highestStep = J2stepsToGo;      
    if(J3stepsToGo > highestStep){
      highestStep = J3stepsToGo;
    }

    float resolutionMultiplier = 4; //maybe motor number * 2

                                                                                //Finding the numbers of steps that each stepper has to skip in order for the moves to be started and stopped simultaneously
    float J2stepSkipF = highestStep / J2stepsToGo * resolutionMultiplier;       //for example stepper 1 is taking a step every J1stepSkip step. IE if J1stepSkip is 4, then stepper 1 takes a step every fourth program loop
    float J3stepSkipF = highestStep / J3stepsToGo * resolutionMultiplier;

    int J2stepSkip = (int)J2stepSkipF;
    int J3stepSkip = (int)J3stepSkipF;

    digitalWrite(J2dirPin, J2dir);
    digitalWrite(J3dirPin, J3dir);

    Serial.print("highestStep:");
    Serial.println(highestStep);
    if(highestStep == 0){
      highestStepInt = 0;
    }
    else highestStepInt = (long)highestStep * resolutionMultiplier;
  
    while(commonCurrentStep < highestStepInt){
      Serial.println(commonCurrentStep);
      if(commonCurrentStep%J2stepSkip==0){
        if(J2currentSteps < J2stepsToGo){
          digitalWrite(J2stepPin,HIGH);
          delayMicroseconds(2);
          digitalWrite(J2stepPin, LOW);
          J2currentSteps++;
        }
      }

      if(commonCurrentStep%J3stepSkip==0){
        if(J3currentSteps < J3stepsToGo){
          digitalWrite(J3stepPin,HIGH);
          delayMicroseconds(2);
          digitalWrite(J3stepPin, LOW);
          J3currentSteps++;
        }
      }
      delayMicroseconds(10);
      commonCurrentStep++;
    }
    
    J2stepsToGo = 0;
    J2currentSteps = 0;
    J2prevSteps = J2newSteps;
    J3stepsToGo = 0;
    J3currentSteps = 0;
    J3prevSteps = J3newSteps;
    commonCurrentStep = 0;
    highestStepInt = 0;

    driveToAngle = false;
  }
}

void jogging(String data) //contorl simple motor
{
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
  
}
