#include <AccelStepper.h>

#define DEG_PER_STEP 5
#define STEP_PER_REVOLUTION (360 / DEG_PER_STEP)

// ESP32 pins: IN1=5, IN2=18, IN3=19, IN4=21
AccelStepper stepper(AccelStepper::FULL4WIRE, 5, 18, 19, 21); 

long angleToSteps(float angle) {
  return (angle / 360.0) * STEP_PER_REVOLUTION;
}

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
  if (Serial.available() > 0) {
    String incomingData = Serial.readStringUntil('\n');
    incomingData.trim();
    int commaIndex = incomingData.lastIndexOf(',');

    // Only proceed if a comma was found
    if (commaIndex != -1) {
      
      String position = incomingData.substring(0, commaIndex);
      String distString = incomingData.substring(commaIndex + 1);
      int distance = distString.toInt();

      Serial.print("Success! position: ");
      Serial.print(position);
      Serial.print(" | distance: ");
      Serial.println(distance);

      // Declare targetSteps outside the if/else chain so moveTo() can see it
      // Defaulting to the current position so it doesn't move if an invalid string is sent
      long targetSteps = stepper.currentPosition(); 

      if(position == "LEFT"){
        // Re-assign targetSteps (don't use the 'long' keyword here again)
        targetSteps = angleToSteps(45);
      }
      else if (position == "CENTER"){
        targetSteps = angleToSteps(0);
      }
      else if (position == "RIGHT"){
        targetSteps = angleToSteps(-45);
      }

      // moveTo is now safely inside the block where targetSteps exists
      stepper.moveTo(targetSteps);
    }
  }
  
  // run() must be called as often as possible outside of the if blocks
  stepper.run();
}