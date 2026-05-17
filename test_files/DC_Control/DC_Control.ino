// Define the control pins
const int motorPin1 = 26;
const int motorPin2 = 12;

void setup() {
  // Initialize the pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
}

void off(){
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
}
void back(){
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
}
void forward(){
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
}

void loop() {
  // ---- Move Forwards at Full Power ----
  forward();
  delay(3000); // Run for 3 seconds

  // ---- Stop / Brake ----
  // It's good practice to briefly stop the motor before reversing 
  // to protect the driver chip and motor from voltage spikes.
  off();
  delay(500); // Pause for 0.5 seconds

  // ---- Move Backwards at Full Power ----
  back();
  delay(3000); // Run for 3 seconds

  // ---- Stop / Brake ----
  off();
  delay(500); // Pause for 0.5 seconds
}
