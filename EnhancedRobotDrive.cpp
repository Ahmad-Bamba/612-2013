#include <RobotDrive.h>
#include "EnhancedRobotDrive.h"
#include "main.h"

EnhancedRobotDrive::EnhancedRobotDrive(SpeedController* a,SpeedController* b,SpeedController* c,SpeedController* d,void* o):
        RobotDrive(a,b,c,d),
        rightShifter(rightShifterInfo.moduleNumber,rightShifterInfo.channel),
        leftShifter(leftShifterInfo.moduleNumber,leftShifterInfo.channel) 
{
    robot_class* robot = (robot_class*)o;
    driver = &(robot -> drive_gamepad);
    gunner = &(robot -> gunner_gamepad);
    robotState = &(robot -> curState);
    driver -> addBtn(DRIVER_SHIFT_LOW,&shiftLowGear,(void*)this);
    driver -> addBtn(DRIVER_SHIFT_HIGH,&shiftHighGear,(void*)this);
    robot -> disableRegistry.addUpdateFunction(&disable,(void*)this);
    robot -> updateRegistry.addUpdateFunction(&update,(void*)this);
    drivePower = 1.0;
}

EnhancedRobotDrive::~EnhancedRobotDrive() {
}

void EnhancedRobotDrive::doControls() {
    if(!(driver -> IsAxisZero(DRIVER_LEFT_DRIVE_AXIS)) || !(driver -> IsAxisZero(DRIVER_RIGHT_DRIVE_AXIS)))
    {
        //Skyler Driving
        TankDrive((driver -> GetRawAxis(DRIVER_LEFT_DRIVE_AXIS)) * drivePower,(driver -> GetRawAxis(DRIVER_RIGHT_DRIVE_AXIS)) * drivePower);
    }
    else if(gunner -> GetTriggerState() != TRIG_NONE && *robotState == robot_class::NORMAL)
    {
        //Ben Swivel
        if(gunner -> GetTriggerState() == GUNNER_SWIVEL_RIGHT)
        {
            swivel(RIGHT);
        }
        else
        {
            swivel(LEFT);
        }
    }
    else if(driver -> GetTriggerState() != TRIG_NONE)
    {
        //Skyler Swivel
        if(driver -> GetTriggerState() == GUNNER_SWIVEL_RIGHT)
        {
            swivel(RIGHT);
        }
        else
        {
            swivel(LEFT);
        }
    }
    else
        TankDrive(0.0f,0.0f);
}

void EnhancedRobotDrive::swivel(dir d) {
    if(d == LEFT)
        TankDrive(DRIVE_TURN_SPEED,-1.0 * DRIVE_TURN_SPEED);
    else
        TankDrive(-1.0 *DRIVE_TURN_SPEED,DRIVE_TURN_SPEED);
}
void EnhancedRobotDrive::disable(void* o) {
    ((EnhancedRobotDrive*)o) -> TankDrive(0.0f,0.0f);
}
void EnhancedRobotDrive::shiftLowGear(void* o) {
    (((EnhancedRobotDrive*)o) -> leftShifter).Set(kleftgear[0]);
    (((EnhancedRobotDrive*)o) -> rightShifter).Set(krightgear[0]);
}
void EnhancedRobotDrive::shiftHighGear(void* o) {
    (((EnhancedRobotDrive*)o) -> leftShifter).Set(kleftgear[1]);
    (((EnhancedRobotDrive*)o) -> rightShifter).Set(krightgear[1]);
}
void EnhancedRobotDrive::update(void* o) {
    EnhancedRobotDrive* thisObject = (EnhancedRobotDrive*)o;
    if(*(thisObject -> robotState) == robot_class::NORMAL)
    {
        thisObject -> drivePower = 1.0;
    }
    else
    {
        thisObject -> drivePower = 0.7;
    }
}
