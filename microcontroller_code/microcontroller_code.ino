#define LED 2


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  pinMode(LED, OUTPUT);
}

void  loop() {
  while (!Serial.available());
  x = Serial.readString();
  if(x == "ON"){
    digitalWrite(LED, LOW); // Turn the LED off
  }
  else if(x == "OFF"){
    digitalWrite(LED, HIGH);
  }
  delay(500);
}

