#pragma once

#include "strategy/Strategy.h"
#include "devices/DeployableDevice.h"
#include "CurtinControllers.h"
#include "Toggle.h"
#include "Gearbox.h"

using BoxIntakeState = curtinfrc::devices::DeployableDeviceState;

struct BoxIntakeConfig : public curtinfrc::devices::DeployableDeviceConfig {
  curtinfrc::Gearbox &motors;

  BoxIntakeConfig(curtinfrc::Gearbox &motorsIn, curtinfrc::actuators::BinaryActuator &actuatorIn, bool canEjectIn = true) : curtinfrc::devices::DeployableDeviceConfig(actuatorIn, canEjectIn), motors(motorsIn) {};
};

class BoxIntake : public curtinfrc::devices::DeployableDevice {
 public:
  BoxIntake(BoxIntakeConfig config) : DeployableDevice(config), _config(config) {};

 protected:
  virtual void IntakingPeriodic() override;  // Intake a cargo
  virtual void OuttakingPeriodic() override; // Eject a cargo
  virtual void DeployingPeriodic() override;
  virtual void StowingPeriodic() override;
  virtual void StowedPeriodic() override;

 private:
  BoxIntakeConfig _config;
};

class BoxIntakeManualStrategy : public curtinfrc::Strategy {
 public:
  BoxIntakeManualStrategy(BoxIntake &boxIntake, curtinfrc::ControllerGroup &contGroup) : Strategy("Box Manual"),  _boxIntake(boxIntake), _contGroup(contGroup) {
    Requires(&boxIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  }

  void OnUpdate(double dt) override;

 private:
  BoxIntake &_boxIntake;
  curtinfrc::ControllerGroup &_contGroup;
};
