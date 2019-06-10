#include <iostream>
#include <cmath>
#include "pid.h"

using namespace std;

// Class Function Implementation
pidControl::pidControl( double dt, double max, double min, double Kp, double Kd, double Ki ) {
    _dt = dt;
    _max = max;
    _min = min;
    _Kp = Kp;
    _Kd = Kd;
    _Ki = Ki;
    _error = 0;     // Error is initially 0
    _integral = 0;  // Integral is initially 0

}

double pidControl::calculate( double setpoint, double pv ){
    double error = setpoint - pv;  // Calculate error

    // Proportional term
    double Pout = _Kp * error;

    // Integral term
    _integral += error * _dt;
    double Iout = _Ki * _integral;

    // Derivative term
    double derivative = (error - _error) / _dt;
    double Dout = _Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if(output > _max)
        output = _max;
    else if(output < _min)
        output = _min;

    // Save error to previous error
    _error = error;

    return output;

}

pidControl::~pidControl() {
    // TODO
}
