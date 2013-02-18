#include <cmath>
#include <Timer.h>
#include "state_driving.h"
#include "../ports.h"
#include "../auto_encoders.h"
#include "../shifter.h"
#include "../drivetrain.h"
#include "../EnhancedJoystick.h"

const float DRIVE_SPEED = 0.2f;
const float ARCADE_SPEED = 0.2f;

void climb_check_update() {
}

void climbing_state() {
    lift.set_angle(-40);
    if(!joyzero(drive_gamepad.GetRawAxis(2))||!joyzero(drive_gamepad.GetRawAxis(4))) // axis controls
    {
        driverOperation = true;
    }
    else if(drive_gamepad.GetRawButton(7)||drive_gamepad.GetRawButton(8))
    {
        driverOperation = true;
    }
    else if(!joyzero(drive_gamepad.GetRawAxis(6))) // arcade drive
    {
        driverOperation = true;
    }
    else
    {
        driverOperation = false;
    }
    if(driverOperation)
    {
        // swiveling
        if (drive_gamepad.GetRawButton(5))// L1 
        {
            drive_train.TankDrive(-DRIVE_SPEED,DRIVE_SPEED);
            return;
        }
        else if(drive_gamepad.GetRawButton(6)) // R1
        {
            drive_train.TankDrive(DRIVE_SPEED,-DRIVE_SPEED);
            return;
        }

        float arcade_y=drive_gamepad.GetRawAxis(6);
        if(!joyzero(arcade_y)) {
            drive_train.TankDrive(arcade_y*ARCADE_SPEED,arcade_y*ARCADE_SPEED);
            return;
        }

        // driving
        float left_axis = drive_gamepad.GetRawAxis(2);
        float right_axis = drive_gamepad.GetRawAxis(4);
        if (joyzero(left_axis) && joyzero(right_axis)) {
            drive_train.TankDrive(0, 0);
        }
        else
        {
            drive_train.TankDrive(left_axis, right_axis);
        }
    }
}
