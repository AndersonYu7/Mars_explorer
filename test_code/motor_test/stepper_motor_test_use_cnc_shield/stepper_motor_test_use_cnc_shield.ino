/*
Arduino引腳  8 ———————– CNC 擴展板 EN     ( 所有電機驅動板始能引腳/底電平有效 )
Arduino引腳  5 ———————– CNC 擴展板 X.DIR  ( X端口方向控制引腳 )
Arduino引腳  2 ———————– CNC 擴展板 X.STEP ( X端口步進控制引腳 )
Arduino引腳  6 ———————– CNC 擴展板 Y.DIR  ( Y端口方向控制引腳 )
Arduino引腳  3 ———————– CNC 擴展板 Y.STEP ( Y端口步進控制引腳 )
Arduino引腳  7 ———————– CNC 擴展板 Z.DIR  ( Z端口方向控制引腳 )
Arduino引腳  4 ———————– CNC 擴展板 Z.STEP ( Z端口步進控制引腳 )
Arduino引腳 13 ———————– CNC 擴展板 A.DIR  ( A端口方向控制引腳 )
Arduino引腳 12 ———————– CNC 擴展板 A.STEP ( A端口步進控制引腳 )

如果需要用到A腳位 需要使用跳線套 套住旁邊針腳 
*/

// defines pins numbers

const int stepX = 2;

const int dirX  = 5;



const int stepY = 3;

const int dirY  = 6;



const int stepZ = 4;

const int dirZ  = 7;


const int stepA = 12;

const int dirA  = 13;



const int enPin = 8;



void setup() {



  // Sets the two pins as Outputs

  pinMode(stepX,OUTPUT);

  pinMode(dirX,OUTPUT);



  pinMode(stepY,OUTPUT);

  pinMode(dirY,OUTPUT);



  pinMode(stepZ,OUTPUT);

  pinMode(dirZ,OUTPUT);

  

  pinMode(stepA,OUTPUT);

  pinMode(dirA,OUTPUT);



  pinMode(enPin,OUTPUT);

  digitalWrite(enPin,LOW);



  digitalWrite(dirX,HIGH);

  digitalWrite(dirY,LOW);

  digitalWrite(dirZ,HIGH);
  
  digitalWrite(dirA,LOW);



}

void step_control(const int step_)
{
  for(int i = 0; i < 800; i++) {

    digitalWrite(step_,HIGH);

    delayMicroseconds(1000);

    digitalWrite(step_,LOW);

    delayMicroseconds(1000);

  }

  delay(1000); // One second delay
}

void loop() {
  step_control(stepX);
  step_control(stepY);
  step_control(stepZ);
  
}
