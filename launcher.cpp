#include <cmath>
#include <Jaguar.h>
#include <PIDController.h>
#include "ports.h"
#include "launcher.h"
#include "612.h"
<<<<<<< HEAD
#ifdef Suzie
=======
#include "NetworkCom.h"

>>>>>>> 4254d6f6dfa4cda97d62d904d6b2317dac77c2e2
Launcher::Launcher(hw_info wheel1,hw_info wheel2,hw_info sensor) : launcherWheel(wheel1,wheel2),
                                                                   launcherSensor(sensor.moduleNumber, sensor.channel),
                                                                   pid(PID_P, PID_I, PID_D, &launcherSensor, &launcherWheel) {
    count = 0;
    targetSpeed = 0;
    targetSet = false;
    updateRegistry.addUpdateFunction(&update_helper,(void*)this);
    launcherSensor.Start();
    pid.Disable();
    pid.SetTolerance(AT_SPEED_TOLERANCE);
    pid.SetInputRange(0.0f, 65.0f);
    pid.SetOutputRange(-0.4f, 0.4f);
    PreviousSpeed = 0;
}
#else
Launcher::Launcher(canport_t info) : launcherWheel(info){
    launcherWheel.SetSpeedReference(CANJaguar::kSpeedRef_Encoder);
    launcherWheel.SetPID(P,I,D);
    launcherWheel.ChangeControlMode(CANJaguar::kSpeed);
    count = 0;
    targetSpeed = 0;
    targetSet = false;
    PreviousSpeed = 0;
    updateRegistry.addUpdateFunction(&update_helper,(void*)this);
}
#endif//Suzie

Launcher::~Launcher() {
}

void Launcher::stop() {
    targetSet = false;
    reachedSpeed = false;
    targetSpeed=0.0f;
    launcherWheel.Set(0.0f);
#ifdef Suzie
    pid.Disable();
#else
    launcherWheel.DisableControl();
#endif //Suzie
    //insert more code here
}

void Launcher::setSpeed(float newSpeed) {
    targetSpeed=newSpeed;
    targetSet=true;
    reachedSpeed = false;
#ifdef Suzie
    launcherWheel.Set(0.3);
    pid.Enable();
    pid.SetSetpoint(newSpeed);
#else
    launcherWheel.EnableControl(newSpeed);
#endif
    
}

float Launcher::getCurrentSpeed() {
#ifdef Suzie
    std::printf("getting current speed when period=%f\n",launcherSensor.GetPeriod());
    return 1.0f/(launcherSensor.GetPeriod());
#else
    return launcherWheel.GetSpeed(); //Todo add setup to constructor
#endif

}

float Launcher::getTargetSpeed() {
    return targetSpeed;
}

bool Launcher::atSpeed(){
    if(std::fabs(getCurrentSpeed()-targetSpeed)/targetSpeed < AT_SPEED_TOLERANCE){
        return true;
    }
    return false; //Replaces OnTarget because it crashes
}

void Launcher::resetFrisbeeCount(){
    count = 0;
}

unsigned int Launcher::getFrisbeeCount(){
    return count;
}
//extern ports.h
void Launcher::update() {
<<<<<<< HEAD
    if(targetSet)
    {
        if(atSpeed())
        {
            reachedSpeed = true;
        }
        if(reachedSpeed)
        {
            if(std::fabs(getCurrentSpeed()-PreviousSpeed) > SHOT_DROP_TOLERANCE)
            {
                count++;
            }
            PreviousSpeed = getCurrentSpeed();
        }
    }
=======
	netcom.launcher_current_speed(getCurrentSpeed());
>>>>>>> 4254d6f6dfa4cda97d62d904d6b2317dac77c2e2
}
//get current speed and angle and send with netcom
void Launcher::update_helper(void* obj) {
    ((Launcher*)obj) -> update();
}
