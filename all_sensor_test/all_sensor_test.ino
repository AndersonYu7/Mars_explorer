typedef struct ultra_sonic
{
  int trig;
  int echo;
  int distance;
}ul;

typedef struct simple_fire_sensor
{
  int Signal;
  int val;
}fire;

ul front_ultra{.trig = 24, .echo = 25};
ul left_ultra{.trig = 26, .echo = 27};
ul right_ultra{.trig = 28, .echo = 29};

fire fires[5] = {
  {.Signal = A2},
  {.Signal = A3},
  {.Signal = A4},
  {.Signal = A5},
  {.Signal = A6}
};

void setup() 
{
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
  
  Serial.begin(9600);
}

int ultra_dis(int trig, int echo)
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  int duration = pulseIn(echo, HIGH);

  return duration * 0.034 / 2;
}

void print_ultra_val(ul ultra)
{
  Serial.print("Distance: ");
  Serial.print(ultra.distance);
  Serial.print(" cm  ");
}

void print_fire_vals(fire fire_sensors[], int size)
{
  for(int i=0;i<size;i++){
    if(i!=size-1){
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(fire_sensors[i].val);
      Serial.print("  ");
    }
    else{
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(fire_sensors[i].val);
      Serial.println("");
    }
  }
}

void loop() 
{
  //display ultra sonic distance (front, left, right)
  front_ultra.distance = ultra_dis(front_ultra.trig, front_ultra.echo);
  left_ultra.distance = ultra_dis(left_ultra.trig, left_ultra.echo);
  right_ultra.distance = ultra_dis(right_ultra.trig, right_ultra.echo);

  Serial.print("Front ");
  print_ultra_val(front_ultra);
  Serial.print("left ");
  print_ultra_val(left_ultra);
  Serial.print("right ");
  print_ultra_val(right_ultra);
  Serial.println("");

  //display the flame sensor's values
  Serial.print("Flame Sensor: ");
  fires[0].val = analogRead(A2);
  fires[1].val = analogRead(A3);
  fires[2].val = analogRead(A4);
  fires[3].val = analogRead(A5);
  fires[4].val = analogRead(A6);
  print_fire_vals(fires, 5);
  delay(1000);
  
}
