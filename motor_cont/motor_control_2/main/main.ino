
// Unit tests
//#define IMU
//#define ENCODER
//#define SPEED_ENCODER
//#define PWM
//#define CONTROLLER

// Single motor integration tests
//#define MOTOR_IDENTIFY  // motors with no controller
//#define MOTOR_INTEGRATED  // motors with controller
//#define MOTOR_LINEAR_SPEED  // motors in m/s

// Combined motor integration tests
//#define ROBOT_DRIVER  // drives robot using v and w

// Odometry tests
//#define ODOMETRY // finds the position of the robot

// Path tracking tests
//#define STANLEY_SIMULATION
//#define STANLEY

// Serial tests
//#define NUC_SERIAL

// Integrated code
#define INTEGRATED_SYSTEM

// Debug statements
//#define DEBUG

// ----Benchmark function----
// Log time between next occurence
#include <NeoHWSerial.h>
void logTime() {
  static uint32_t prevTime, currTime;
  prevTime = currTime;
  currTime = micros();
  NeoSerial.println(currTime-prevTime);
}

// --- PARAMETERS ---
// PIDV Parameters; They set the response time to a desired linear velocity
// PID gain values
#define PID_V_KP    0.4
#define PID_V_KI    0.02
#define PID_V_KD    0.000

// PID output saturation values
#define PID_V_MIN   -1.0
#define PID_V_MAX   1.0

// PID integral windup saturation value
#define PID_V_IMAX  0.0

// PIDW Parameters; They set the response time to a desired angular velocity
// PID gain values
#define PID_W_KP    5.0
#define PID_W_KI    0.0
#define PID_W_KD    0.0

// PID output saturation values
#define PID_W_MIN   -2.0
#define PID_W_MAX   2.0

// PID integral windup saturation value
#define PID_W_IMAX  0.0
