int x, y;
String data;
int ind1;

void setup() {
 Serial.begin(115200);
 Serial.setTimeout(1);
}
void loop() {
 while (!Serial.available());
  data = Serial.readStringUntil('\r');
  ind1 = data.indexOf(',');
  x = data.substring(0, ind1).toInt();
  y = data.substring(ind1+1).toInt();

  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
}
