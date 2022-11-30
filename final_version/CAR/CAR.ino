#include "Wire.h"       
#include "I2Cdev.h"     
#include "MPU6050.h"  

int Value = 100;  // 馬達轉速
int Value4 = 50;  // 馬達轉速
int angle = -100; // 讀取伺服馬達轉角（預設為-100）
int safe_dis = 20;// 距離障礙物的最小安全距離
int target_dis;   // 與目標物的距離
int input_gyro;   // 陀螺儀感測器的角度
int gyro = 0;     // 目標轉角
String x_y;       // 接收Serial的字串
int x;            // 距離圖片中心的X座標
int y;            // 距離圖片中心的座標
int arm_finish_IN;// 接收手臂動作是否結束



MPU6050 mpu;      // 陀螺儀感測器宣告
int16_t ax, ay, az;
int16_t gx, gy, gz;

struct MyData {
  byte X;
  byte Y;
};

MyData data;

typedef struct ultra_sonic // 超音波感測器宣告
{
  int trig;
  int echo;
  int distance;
}ul;

typedef struct simple_fire_sensor // 火焰感測器宣告
{
  int Signal;
  int val;
}fire;

ul front_ultra{.trig = 24, .echo = 25}; // 超音波感測器腳位宣告（前）
ul left_ultra{.trig = 26, .echo = 27};  // 超音波感測器腳位宣告（左）
ul right_ultra{.trig = 28, .echo = 29}; // 超音波感測器腳位宣告（右）

fire fires[5] = { // 火焰感測器腳位宣告
  {.Signal = A2},
  {.Signal = A3},
  {.Signal = A4},
  {.Signal = A5},
  {.Signal = A6}
};



//2,4,6,8正轉 / 3,5,7,9反轉                      
void Forward() // 前
 {
  analogWrite(2, Value);
  analogWrite(3, 0);
  analogWrite(4, Value);
  analogWrite(5, 0);
  analogWrite(6, Value);
  analogWrite(7, 0);
  analogWrite(8, Value);
  analogWrite(9, 0);
 }
void Back() // 後
 {
  analogWrite(2, 0);
  analogWrite(3, Value);
  analogWrite(4, 0);
  analogWrite(5, Value);
  analogWrite(6, 0);
  analogWrite(7, Value);
  analogWrite(8, 0);
  analogWrite(9, Value);
 }
void Left() // 左上
 {
  analogWrite(2, Value);
  analogWrite(3, 0);
  analogWrite(4, Value);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(7, Value);
  analogWrite(8, 0);
  analogWrite(9, Value);
 }
void Right() // 右上
 {
  analogWrite(2, 0);
  analogWrite(3, Value);
  analogWrite(4, 0);
  analogWrite(5, Value);
  analogWrite(6, Value);
  analogWrite(7, 0);
  analogWrite(8, Value);
  analogWrite(9, 0);
 }
void STOP(){ // 停止
  analogWrite(2, 0);
  analogWrite(3, 0);
  analogWrite(4, 0);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(7, 0);
  analogWrite(8, 0);
  analogWrite(9, 0);
  
}

void Rightup(){           //右平移
  analogWrite(2, 0);      //左後
  analogWrite(3, Value4); //左後
  analogWrite(4, Value);  //左前
  analogWrite(5, 0);      //左前
  analogWrite(6, 0);      //右前
  analogWrite(7, Value4); //右前
  analogWrite(8, Value);  //右後
  analogWrite(9, 0);      //右後
}
void Leftup(){            //左平移
  analogWrite(2, Value);  //左後
  analogWrite(3, 0);      //左後
  analogWrite(4, 0);      //左前
  analogWrite(5, Value4); //左前
  analogWrite(6, Value);  //右前
  analogWrite(7, 0);      //右前
  analogWrite(8, 0);      //右後
  analogWrite(9, Value4); //右後
}


int ultra_dis(int trig, int echo) //計算超音波的距離
{
  // Clears the trigPin condition
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(echo, HIGH);
  
  // Calculating the distance
  return duration * 0.034 / 2;
}



 void Left_down_pivot_go_left(){ //左下為支點,向左轉
  analogWrite(2, 0);      //左後
  analogWrite(3, 0);      //左後
  analogWrite(4, 0);      //左前
  analogWrite(5, 0);      //左前
  analogWrite(6, Value);  //右前
  analogWrite(7, 0);      //右前
  analogWrite(8, Value);  //右後
  analogWrite(9, 0);      //右後
}

void Right_down_pivot_go_right(){ //右下為支點,向右轉
  analogWrite(2, Value);  //左後
  analogWrite(3, 0);      //左後
  analogWrite(4, Value);  //左前
  analogWrite(5, 0);      //左前
  analogWrite(6, 0);      //右前
  analogWrite(7, 0);      //右前
  analogWrite(8, 0);      //右後
  analogWrite(9, 0);      //右後
}

int detect_gyro(){ //讀取陀螺儀角度
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    data.X = map(ax, -17000, 17000, 0, 255 ); // X axis data
    data.Y = map(ay, -17000, 17000, 0, 255);  // Y axis data
    return data.X-130; // data.X-130 > 0:往左轉
}


void setup() {  
  pinMode(front_ultra.trig, OUTPUT);
  pinMode(front_ultra.echo, INPUT);
  pinMode(left_ultra.trig, OUTPUT);
  pinMode(left_ultra.echo, INPUT);
  pinMode(right_ultra.trig, OUTPUT);
  pinMode(right_ultra.echo, INPUT);

  pinMode(fires[0].Signal, INPUT);
  pinMode(fires[1].Signal, INPUT);
  pinMode(fires[2].Signal, INPUT);
  pinMode(fires[3].Signal, INPUT);
  pinMode(fires[4].Signal, INPUT);
  Wire.begin();
  //mpu.initialize();     //                      input                         output
  Serial.begin(115200);  //for computer (string) {x,y}                         (int)    {0/1(arm finish?)}
  Serial2.begin(115200);  //for arm_MEGA (int)    {0/1(arm finish?)}            (int)    {target_dis}
  Serial1.begin(115200);   //for ESP32    (int)    {servo_angle}                 (string) {0/1(arm finish?),x,y}(if x,y undetect then x=-1,y=-1)
}

void loop() {

  while(1){
    if(Serial.available()) x_y = Serial.readString(); //讀取串口S
    char ind1 = x_y.indexOf(','); 
    x = x_y.substring(0,ind1).toInt();
    y = x_y.substring(ind1+1).toInt();

    
    if(Serial2.available()) arm_finish_IN = Serial2.read(); //讀取串口S2
    String arm_x_y = String(arm_finish_IN)+','+String(x)+','+String(y);
    Serial.write(arm_x_y);        //輸出串口S
    Serial2.write(arm_finish_IN); //輸出串口S2

    
    if(Serial1.available()) angle = Serial1.read(); //讀取串口S1
  
    
    


    
    if(angle == -100){ //若沒有收到伺服馬達轉角：直行
        front_ultra.distance = ultra_dis(front_ultra.trig, front_ultra.echo); //讀取前方距離
        left_ultra.distance = ultra_dis(left_ultra.trig, left_ultra.echo);    //讀取左方距離
        right_ultra.distance = ultra_dis(right_ultra.trig, right_ultra.echo); //讀取右方距離
        int go_right = 1;
        int go_left = 0;

          while(front_ultra.distance < safe_dis){ //若前方小於安全距離
            if(Serial1.available()){ //監控是否有偵測到物體
              angle = Serial1.read();
              if(angle != -100) goto HAVE_ANGLE; //偵測到跳至HAVE_ANGLE
            }
            if(go_right){ //一開始設定前方碰到障礙：往右平移
              Rightup();  //右平移
              right_ultra.distance = ultra_dis(right_ultra.trig, right_ultra.echo);  
              if(right_ultra.distance < safe_dis){ //若右方小於安全距離：左平移
                go_left = 1;
                go_right = 0;            
              }
            }
            if(go_left){ //往左平移
              Leftup();
              left_ultra.distance = ultra_dis(right_ultra.trig, right_ultra.echo);  
              if(left_ultra.distance < safe_dis){ //若左方小於安全距離：後退
                go_left = 0;
                go_right = 0;            
              }
            }

            if(go_left == 0 && go_right == 0){ //退後
              while(left_ultra.distance < safe_dis){ //如果左邊沒有缺口：一直後退
                left_ultra.distance = ultra_dis(right_ultra.trig, right_ultra.echo); 
                Back();
              }
              Leftup(); //找到缺口後：往左一小段
              delay(1000);
              while(front_ultra.distance < safe_dis){ //所果前方有障礙：一直往左平移
                Leftup();
              }
              break;
            }
            
          }
          while(left_ultra.distance < safe_dis){ //若左方小於安全距離：右平移
            if(Serial1.available()>0){ //監控是否有偵測到物體
              angle = Serial1.read();
              if(angle != -100) goto HAVE_ANGLE; //偵測到跳至HAVE_ANGLE
            }
            Rightup();
          }
          while(right_ultra.distance < safe_dis){ //若右方小於安全距離：左平移
            if(Serial1.available()>0){ //監控是否有偵測到物體
              angle = Serial1.read();
              if(angle != -100) goto HAVE_ANGLE; //偵測到跳至HAVE_ANGLE
            }
            Leftup();
          }
          Forward(); //都沒有障礙,直走
                 
    }else{// camera capture
      HAVE_ANGLE:
      STOP(); //暫停
      gyro = 90 - angle;//gyro > 0:left,<0:right
      
      input_gyro = detect_gyro(); //讀取陀螺儀角度
      while(input_gyro != gyro){  //若陀螺儀角度不等於伺服馬達角度：車子轉動
        input_gyro = detect_gyro();
        if(gyro < 0){ //右旋轉
          Right_down_pivot_go_right();
        }else{ //左旋轉
          Left_down_pivot_go_left();
        }
      }
      STOP();
      target_dis = ultra_dis(front_ultra.trig, front_ultra.echo); //讀取與目標物的距離
      Serial2.write(target_dis); //輸出距離
      
      if(Serial2.available()) arm_finish_IN = Serial2.read(); //讀取手臂是否結束運動
      while(arm_finish_IN != '1'){ //若還沒結束運動：等待運動完成
        if(Serial2.available()) arm_finish_IN = Serial2.read();
      }

      input_gyro = detect_gyro();
      while(input_gyro != 0){ //轉回原位
        input_gyro = detect_gyro();
        if(input_gyro < 0){
          Left_down_pivot_go_left();
        }else{
          Right_down_pivot_go_right();
        }        
      }    
    }

  }
}

