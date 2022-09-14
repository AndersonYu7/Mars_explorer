//receive the data from esp32-cam

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("POS:");
  Serial.println(Serial.read());
}
