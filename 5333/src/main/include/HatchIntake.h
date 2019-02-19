#pragma once

#include "strategy/Strategy.h"
#include "devices/DeployableDevice.h"
#include "actuators/BinaryActuator.h"
#include "CurtinControllers.h"
#include "Toggle.h"

using HatchIntakeState = curtinfrc::devices::DeployableDeviceState;

struct HatchIntakeConfig : public curtinfrc::devices::DeployableDeviceConfig {
  curtinfrc::actuators::BinaryActuator &manipulator;
  curtinfrc::actuators::BinaryActuatorState stowedState;

  HatchIntakeConfig(curtinfrc::actuators::BinaryActuator &manipulatorIn, curtinfrc::actuators::BinaryActuator &actuatorIn, curtinfrc::actuators::BinaryActuatorState stowedStateIn = curtinfrc::actuators::kForward, bool canEjectIn = false) : curtinfrc::devices::DeployableDeviceConfig(actuatorIn, canEjectIn), stowedState(stowedStateIn), manipulator(manipulatorIn) {};
};

class HatchIntake : public curtinfrc::devices::DeployableDevice {
 public:
  HatchIntake(HatchIntakeConfig config) : DeployableDevice(config), _config(config) {};

  HatchIntakeConfig &GetConfig() { return _config; };

 protected:
  virtual void IntakingPeriodic() override;  // Intake a hatch
  virtual void OuttakingPeriodic() override; // Eject a hatch
  virtual void StowedPeriodic() override;

 private:
  HatchIntakeConfig _config;
};

class HatchIntakeManualStrategy : public curtinfrc::Strategy {
 public:
  HatchIntakeManualStrategy(HatchIntake &hatchIntake, curtinfrc::ControllerGroup &contGroup, bool startEnabled) : Strategy("Hatch Manual"),  _hatchIntake(hatchIntake), _contGroup(contGroup), _enabledToggle(curtinfrc::ONRISE), _enabled(startEnabled) {
    Requires(&hatchIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  }

  void OnUpdate(double dt) override;

 private:
  HatchIntake &_hatchIntake;
  curtinfrc::ControllerGroup &_contGroup;
  curtinfrc::Toggle _enabledToggle;
  bool _enabled;
};
