#ifdef INTEGRATED_SYSTEM

// Libraries
#include "src/RobotBase.h"
#include "src/Stanley.h"
#include "src/Euler.h"
#include "src/PathReader.h"

// Robot
RobotBase robot;

// Odometry
Pose pose;
Euler ode;

// PID the input values to achieve a slower response
// The motor control should have close to optimal response
PID pidV, pidW;

// EMERGENCY STOP

void setup() {

  NeoSerial.begin(115200);

  // Serial debug
  #ifdef DEBUG
    debugSerial.println(F("Starting program"));
  #endif

  // Flush buffer
  //while(NeoSerial.available()) NeoSerial.read();

  // Led debug
  pinMode(EMERGENCY_STOP_PIN, INPUT);

  // Setup PID for v and w
  pidV = PID(PID_V_KP, PID_V_KI, PID_V_KD, PID_V_MIN, PID_V_MAX, PID_V_IMAX);
  pidW = PID(PID_W_KP, PID_W_KI, PID_W_KD, PID_W_MIN, PID_W_MAX, PID_W_IMAX);

  // Setup robot
  robot.begin();
  robot.setSpeed(0, 0);
}

// State machine for motor controllers
uint8_t stateTrack, statePoseSend;

float vInput = 0.0;
float wInput = 0.0;
float vDesired = 0.0;
float wDesired = 0.0;

String inString[2] = {""};
bool updateControl = true;

void loop() {
  static uint32_t sampleLast = millis();
  // Update pose and control speed
  if(robot.sampleData()) {
    // Control speed
    robot.drive();

    #ifdef DEBUG  // Print data
      logTime();
    #endif

    updateControl = true;
  }

  if (NeoSerial.available() > 0) {
    delay(1);
    int strNum = 0;
    while (NeoSerial.available() > 0) {
      int inChar = NeoSerial.read();
      if (isDigit(inChar) || inChar == '.') {
        // convert the incoming byte to a char and add it to the string:
        inString[strNum] += (char)inChar;
      }
      if (inChar == ',') {
        strNum = 1;
      }
      // if you get a newline, print the string, then the string's value:
      if (inChar == '\n') {
        NeoSerial.print("Value:");
        NeoSerial.print(inString[0].toFloat());
        NeoSerial.print(",");
        NeoSerial.print(inString[1].toFloat());
        NeoSerial.print(" String: ");
        NeoSerial.print(inString[0]);
        NeoSerial.print(",");
        NeoSerial.println(inString[1]);

        // Set the recorded values
        vInput = inString[0].toFloat();
        wInput = inString[1].toFloat();
        
        // clear the string for new input:
        inString[0] = "";
        inString[1] = "";

      }
    }
  }

  // Update control signals
  if(updateControl) {
    // Run controller based on inputs
    vDesired += pidV.pid(vInput-vDesired);
    wDesired += pidW.pid(wInput-wDesired);
    
    // Set speed inputs
    if(!digitalRead(EMERGENCY_STOP_PIN)) {
      robot.setSpeed(vDesired, wDesired);
    }

    NeoSerial.print("Desired: ");
    NeoSerial.print(vDesired);
    NeoSerial.print(" ");
    NeoSerial.println(wDesired);

    NeoSerial.print("Inputs: ");
    NeoSerial.print(vInput);
    NeoSerial.print(" ");
    NeoSerial.println(wInput);
    updateControl = false;
  }

  if(digitalRead(EMERGENCY_STOP_PIN)) {
    robot.setSpeed(0,0);
  }
}

// Prints data
void printData() {
  logTime();
  debugSerial.println();  
}

#endif
