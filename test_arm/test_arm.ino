#define J2stepPin 54      //x
#define J2dirPin 55
#define J2_ENABLE_PIN 38
#define J2limitSwitch 32

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

  J2isCalibrated = false;
  J2jogging = false;

  Serial.begin(115200);
}

//we need to know what is skip step, and it's undone
void drive(String data)//control all motors
{
  Serial.println("driving");
  if(data.startsWith("drive")){ 

    ind1 = data.indexOf(',');
    J2newSteps = data.substring(ind1+1).toFloat();

    data = ""; //data turns into empty string
    driveToAngle = true;
  }

  if(data.startsWith("stop")) driveToAngle = false;
  if(driveToAngle == true){
    if(J2newSteps < J2prevSteps){
      J2dir = 0; //counterclockwise
      J2stepsToGo = J2prevSteps - J2newSteps;
    }
    if(J2newSteps > J2prevSteps){
      J2dir = 1; //clockwise
      J2stepsToGo = J2prevSteps - J2newSteps;
    }

    float highestStep = J2stepsToGo;
    if(J2stepsToGo > highestStep) highestStep = J2stepsToGo;

    float resolutionMultiplier = 10;

    float J2stepSkipF = highestStep / J2stepsToGo * resolutionMultiplier;

    int J2stepSkip = (int)J2stepSkipF;

    digitalWrite(J2dirPin, J2dir);

    if(highestStep == 0) highestStepInt = 0;
    else highestStepInt = (long)highestStep * resolutionMultiplier;

    while(commonCurrentStep < highestStepInt){
      if(commonCurrentStep%J2stepSkip==0)
      {
        if(J2currentSteps < J2stepsToGo)
        {
          digitalWrite(J2stepPin,HIGH);
          delayMicroseconds(2);
          digitalWrite(J2stepPin, LOW);
          J2currentSteps++;
        }
        delayMicroseconds(10);
        commonCurrentStep++;
    }

    J2stepsToGo = 0;
    J2currentSteps = 0;
    J2prevSteps = J2newSteps;

    driveToAngle = false;
  }
}

}

void jogging(String data) //contorl simple motor
{
  Serial.println("jogging");
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

  if(J2jogging == true)
  {
      if(J2dir == 1 && J2prevSteps < J2maxSteps) //clockwise
      {
        J2prevSteps++;

        digitalWrite(J2stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J2stepPin, LOW);
      }
      if(J2dir == 0 && J2minSteps < J2prevSteps) //counterclockwise
      {
        J2prevSteps--;

        digitalWrite(J2stepPin,HIGH);
        delayMicroseconds(2000); //speed
        digitalWrite(J2stepPin, LOW);
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
  

  Serial.println(J2prevSteps);
}
