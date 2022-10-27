int Value = 100;
int Value4 = 50;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

//2,4,6,8正轉 / 3,5,7,9反轉                      
void Forward() 
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
void Back() 
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
void Right_Slide() 
 {
  analogWrite(2, 0);
  analogWrite(3, Value);
  analogWrite(4, Value);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(7, Value);
  analogWrite(8, Value);
  analogWrite(9, 0);
 }
void Left_Slide() 
 {
  analogWrite(2, Value);
  analogWrite(3, 0);
  analogWrite(4, 0);
  analogWrite(5, Value);
  analogWrite(6, Value);
  analogWrite(7, 0);
  analogWrite(8, 0);
  analogWrite(9, Value);
 }
void Left() 
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
void Right() 
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
void Lin() 
 {
  analogWrite(2, 0);
  analogWrite(3, 200);
  analogWrite(4, 200);
  analogWrite(5, 0);
 }
void Rin() 
 {
  analogWrite(6, 200);
  analogWrite(7, 0);
  analogWrite(8, 0);
  analogWrite(9, 200);
 }
 void Lout() 
 {
  analogWrite(2, 200);
  analogWrite(3, 0);
  analogWrite(4, 0);
  analogWrite(5, 200);
 }
void Rout() 
 {
  analogWrite(6, 0);
  analogWrite(7, 200);
  analogWrite(8, 200);
  analogWrite(9, 0);
 }
  void Lstop() 
 {
  analogWrite(2, 0);
  analogWrite(3, 0);
  analogWrite(4, 0);
  analogWrite(5, 0);
 }
 void Rstop() 
 {
  analogWrite(6, 0);
  analogWrite(7, 0);
  analogWrite(8, 0);
  analogWrite(9, 0);
 }
 void Rightup(){
  analogWrite(2, 0);//左後
  analogWrite(3, Value4);//左後
  analogWrite(4, Value);//左前
  analogWrite(5, 0);//左前
  analogWrite(6, 0);//右前
  analogWrite(7, Value4);//右前
  analogWrite(8, Value);//右後
  analogWrite(9, 0);//右後
}
void Leftup(){
  analogWrite(2, Value);//左後
  analogWrite(3, 0);//左後
  analogWrite(4, 0);//左前
  analogWrite(5, Value4);//左前
  analogWrite(6, Value);//右前
  analogWrite(7, 0);//右前
  analogWrite(8, 0);//右後
  analogWrite(9, Value4);//右後
}
void Rightdown(){
  analogWrite(2, 0);//左後
  analogWrite(3, Value);//左後
  analogWrite(4, Value4);//左前
  analogWrite(5, 0);//左前
  analogWrite(6, 0);//右前
  analogWrite(7, Value);//右前
  analogWrite(8, Value4);//右後
  analogWrite(9, 0);//右後
}
void Leftdown(){
  analogWrite(2, Value4);//左後
  analogWrite(3, 0);//左後
  analogWrite(4, 0);//左前
  analogWrite(5, Value);//左前
  analogWrite(6, Value4);//右前
  analogWrite(7, 0);//右前
  analogWrite(8, 0);//右後
  analogWrite(9, Value);//右後
}
void rightzero(){                            //右邊輪子歸0 往右轉
  analogWrite(2, Value);//左後
  analogWrite(3, 0);//左後
  analogWrite(4, Value);//左前
  analogWrite(5, 0);//左前
  analogWrite(6, 0);//右前
  analogWrite(7, 0);//右前
  analogWrite(8, 0);//右後
  analogWrite(9, Value4);//右後
}
void leftzero(){                            //左邊輪子歸0 往左轉
  analogWrite(2, 0);//左後
  analogWrite(3, Value4);//左後
  analogWrite(4, 0);//左前
  analogWrite(5, 0);//左前
  analogWrite(6, Value);//右前
  analogWrite(7, 0);//右前
  analogWrite(8, Value);//右後
  analogWrite(9, 0);//右後
}
void backrightzero(){                            //往後右邊輪子歸0 往右轉
  analogWrite(2, 0);//左後
  analogWrite(3, Value);//左後
  analogWrite(4, 0);//左前
  analogWrite(5, Value);//左前
  analogWrite(6, Value4);//右前
  analogWrite(7, 0);//右前
  analogWrite(8, 0);//右後
  analogWrite(9, 0);//右後
}
void backleftzero(){                            //往後左邊輪子歸0 往左轉
  analogWrite(2, 0);//左後
  analogWrite(3, 0);//左後
  analogWrite(4, Value4);//左前
  analogWrite(5, 0);//左前
  analogWrite(6, 0);//右前
  analogWrite(7, Value);//右前
  analogWrite(8, 0);//右後
  analogWrite(9, Value);//右後
}
void autoright(float a){                            //右邊輪子歸0 往右轉
  analogWrite(2, Value);//左後
  analogWrite(3, 0);//左後
  analogWrite(4, Value);//左前
  analogWrite(5, 0);//左前
  analogWrite(6, 0);//右前
  analogWrite(7, 0);//右前
  analogWrite(8, 0);//右後
  analogWrite(9, Value4);//右後
  delay(a*1000);
}
void autoforward(float b) 
 {
  analogWrite(2, Value);
  analogWrite(3, 0);
  analogWrite(4, Value);
  analogWrite(5, 0);
  analogWrite(6, Value);
  analogWrite(7, 0);
  analogWrite(8, Value);
  analogWrite(9, 0);
  delay(b*1000);
 }

void loop() {
  // put your main code here, to run repeatedly:
  Forward();
  delay(2000);
  Back();
  delay(2000);
  Right_Slide();
  delay(2000);
  Right();
  delay(2000);
  
}
