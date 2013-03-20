#include <cmath>
#include "EnhancedShooter.h"
#include "main.h"

EnhancedShooter::EnhancedShooter(int w,int L,hw_info f,hw_info c,void* o):
        wheel(w),
        lift(L),
        feeder(f.moduleNumber,f.channel),HalEffect(c.moduleNumber,c.channel) ,
        wheelCount(WHalEffectInfo.moduleNumber,WHalEffectInfo.channel),
        wheelCommandCenter(WHEEL_P,WHEEL_I,WHEEL_D,&wheelCount,&wheel)
{
    (((robot_class*)o) -> updateRegistry).addUpdateFunction(&update_helper,(void*)this);
    driver = &((robot_class*)o) -> drive_gamepad;
    gunner = &((robot_class*)o) -> gunner_gamepad;
    robotState = &((robot_class*)o) -> curState;
    HalEffect.Start();
}
void EnhancedShooter::setWheelPower(float speed) {
    wheelCommandCenter.Disable();
    wheel.ChangeControlMode(CANJaguar::kPercentVbus);
    wheel.Set(speed);
}
void EnhancedShooter::setFeeder(direction dir) {
    feeder.Set(dir * FEEDER_SPEED);
}
void EnhancedShooter::setLiftPower(float pwr) {
    lift.ChangeControlMode(CANJaguar::kPercentVbus);
    lift.Set(pwr);
}
void EnhancedShooter::update() {
    if(*robotState == robot_class::NORMAL)
    {
        if(HalEffect.Get() > 0)
        {
            stopFeeder();
            HalEffect.Reset();
            HalEffect.Start();
        }
    }
    else
    {
        setLiftPower(-1 * LIFT_POWER); //Limit Switch Will Stop It
        stopFeeder();
        stopWheel();
    }
}
void EnhancedShooter::stopAll() {
    stopWheel();
    stopFeeder();
    stopLift();
}

void EnhancedShooter::stopWheel() {
    wheelCommandCenter.Disable();
    wheel.DisableControl();
    wheel.ChangeControlMode(CANJaguar::kVoltage);
    wheel.Set(0.0f);
}
void EnhancedShooter::stopLift() {
    lift.DisableControl();
    lift.ChangeControlMode(CANJaguar::kVoltage);
    lift.Set(0.0f);
}
void EnhancedShooter::stopFeeder() {
    feeder.Set(0.0f);
}
void EnhancedShooter::doControls() {
    if(*robotState != robot_class::NORMAL)
        return; // NO conrols in climbing mode
    //Wheel Control
    if(gunner -> GetRawButton(GUNNER_BTN_SHOOTER_WHEEL))
    {
        setSpeed(60.0);
    }
    else if(gunner -> GetRawButton(GUNNER_BTN_SHOOTER_WHEEL_REV))
    {
        setSpeed(-60.0);
    }
    else
    {
        stopWheel();
    }
    //Lift Control
    if(gunner -> GetRawButton(GUNNER_BTN_LIFT_UP))
    {
        setLiftPower(LIFT_POWER);
    }
    else if(gunner -> GetRawButton(GUNNER_BTN_LIFT_DOWN))
    {
        setLiftPower(-1 * LIFT_POWER);
    }
    else
    {
        stopLift();
    }
    //Feeder Controls
    if(gunner -> GetRawAxis(GUNNER_AXIS_FEEDER) < -0.98f) // right
    {
        setFeeder(FORWARD);
    }
    else if(gunner -> GetRawAxis(GUNNER_AXIS_FEEDER) > 0.98f) // left
    {
        setFeeder(BACKWARD);
    }
}
void EnhancedShooter::setAngle(float newTarget) {
    lift.EnableControl();
    lift.ChangeControlMode(CANJaguar::kPosition);
    lift.SetPositionReference(CANJaguar::kPosRef_Potentiometer);
    lift.SetPID(LIFT_P,LIFT_I,LIFT_D);
    newTarget = liftAngleToPot(newTarget);
    lift.Set(newTarget);
}
void EnhancedShooter::setSpeed(float newTarget) {
    wheelCommandCenter.Enable();
    wheelCommandCenter.SetOutputRange(-1.00,1.00);
    wheelCommandCenter.SetSetpoint(newTarget);
}
bool EnhancedShooter::atAngle(float target) {
    //Todo Add Switch For Switches
    target = liftAngleToPot(target);
    if(target <= MIN_POT_VAL)
        return lift.GetForwardLimitOK();
    else if(target >= MAX_POT_VAL)
        return lift.GetReverseLimitOK();
    if(std::fabs(lift.Get() - target) < LIFT_TOLERANCE)
        return true;
    return false;
}
bool EnhancedShooter::atSpeed(float target) {
    if(std::fabs(wheelCommandCenter.Get() - target) < WHEEL_TOLERANCE)
        return true;
    return false;
}
float EnhancedShooter::liftAngleToPot(float angle) {
    //Todo insert Equation Here
    return 5.0f;
}
void EnhancedShooter::update_helper(void* o) {
    ((EnhancedShooter*)o) -> update();
}
void EnhancedShooter::disable(void* o) {
    ((EnhancedShooter*)o) -> stopAll();
}
