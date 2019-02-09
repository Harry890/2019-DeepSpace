#include "control/PIDController.h"

#include <cmath>
#include <frc/smartdashboard/SendableBase.h>
#include <frc/smartdashboard/SendableBuilder.h>

#include <networktables/NetworkTableInstance.h>

#include <iostream>

using namespace curtinfrc::control;

// PIDGains

PIDGains::PIDGains(std::string name, double kP, double kI, double kD, double kF) : _name(name), _kP(kP), _kI(kI), _kD(kD), _kF(kF) {
  _table = nt::NetworkTableInstance::GetDefault().GetTable("PIDGains[" + name + "]");

  _ntbounds.emplace_back(_table, "kP", &_kP);
  _ntbounds.emplace_back(_table, "kI", &_kI);
  _ntbounds.emplace_back(_table, "kD", &_kD);
  _ntbounds.emplace_back(_table, "kF", &_kF);
}

double PIDGains::GetkP() const {
  return _kP;
}

double PIDGains::GetkI() const {
  return _kI;
}

double PIDGains::GetkD() const {
  return _kD;
}

double PIDGains::GetkF() const {
  return _kF;
}

// PIDController

PIDController::PIDController(PIDGains gains, double setpoint) : _gains(gains), _setpoint(setpoint), _lastError(0) {}

void PIDController::SetSetpoint(double setpoint) {
  Reset();
  _setpoint = setpoint;
}

void PIDController::SetIZone(double threshold) {
  _threshold = threshold;
}

double PIDController::GetSetpoint() {
  return _setpoint;
}

void PIDController::SetWrap(double range) {
  _wrapRange = range;
}

double PIDController::Calculate(double processVariable, double dt, double feedforward = 0.0) {
  double error = Wrap(_setpoint - processVariable);
  if (std::abs(error) > _threshold) _integral = 0; // I zone
  else _integral += error * dt; // Calc I
  _derivative = dt > 0 ? (error - _lastError) / dt : 0; // Calc D

  double output = _gains.GetkP() * error + _gains.GetkI() * _integral + _gains.GetkD() * _derivative + _gains.GetkF() * feedforward;
  _lastError = error;

  return output;
}

void PIDController::Reset() {
  _integral = 0;
  _derivative = 0;
  _lastError = 0;
  // Does not reset _threshold, use SetIZone instead
}

double PIDController::Wrap(double val) {
  if (_wrapRange > 0) {
    val = std::fmod(val, _wrapRange);
    if (std::abs(val) > (_wrapRange / 2.0)) {
      return (val > 0) ? val - _wrapRange : val + _wrapRange;
    }
  }

  return val;
}