/*
 * This ESP32 code is created by esp32io.com
 * Optimized to prevent loop blocking
 */

#include <AccelStepper.h>

#define DEG_PER_STEP 5
#define STEP_PER_REVOLUTION (360 / DEG_PER_STEP)

// ESP32 pins: IN1=5, IN2=18, IN3=19, IN4=21
AccelStepper stepper(AccelStepper::FULL4WIRE, 5, 18, 19, 21); 

long moveToPosition = STEP_PER_REVOLUTION;
unsigned long lastPrintTime = 0; // For non-blocking serial print timer

void setup() {
  Serial.begin(115200); // Upgraded speed from 9600 for faster execution
  Serial.println("Starting now");

  stepper.setMaxSpeed(1000.0);    // CRITICAL: You must set MaxSpeed for AccelStepper to work right
  stepper.setAcceleration(800.0); // set acceleration
  stepper.setSpeed(500);          // set initial speed
  stepper.setCurrentPosition(0);  // set position to 0

  stepper.moveTo(STEP_PER_REVOLUTION); // move motor one revolution
  Serial.println("Motor moving in clockwise direction");
}

void loop() {
  // Check if motor reached its target
  if (stepper.distanceToGo() == 0) {
    Serial.println("Motor is stopped");
    
    // Instead of delay(1000) which freezes everything, we handle the direction swap instantly here.
    // If you want an actual pause, it requires a non-blocking timer, but let's fix the speed first!
    
    stepper.setCurrentPosition(0);        // reset position to 0
    moveToPosition = -1 * moveToPosition; // reverse direction
    stepper.moveTo(moveToPosition);       // move motor one revolution

    if (moveToPosition > 0)
      Serial.println("Motor moving in clockwise direction");
    else
      Serial.println("Motor moving in anticlockwise direction");
  }

  // FIX: Only print the position every 200ms instead of every loop cycle
  if (millis() - lastPrintTime >= 200) {
    Serial.print(F("position: "));
    Serial.println(stepper.currentPosition());
    lastPrintTime = millis();
  }

  stepper.run(); // Now this can run at maximum speed!
}