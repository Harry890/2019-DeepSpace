#pragma once

#include <frc/TimedRobot.h>
#include <frc/XboxController.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>

#include "curtin_ctre.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  frc::XboxController *xbox1, *xbox2;
  frc::Spark *left_motor, *right_motor;
  frc::Spark *Cargo;
  frc::Spark *Rotation;
  frc::DoubleSolenoid *Hatch_deploy;
};
