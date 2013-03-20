#ifndef ENHANCED_ROBOT_DRIVE_H
#define ENHANCED_ROBOT_DRIVE_H

#include <RobotDrive.h>
#include <SpeedController.h>
#include <Servo.h>
#include "Controls.h"
#include "Hardware.h"
#include "EnhancedJoystick.h"
#include "main.h"

class EnhancedRobotDrive : public RobotDrive {
public:
    static const float DRIVE_TURN_SPEED = 0.7f;
    EnhancedRobotDrive(SpeedController*,SpeedController*,SpeedController*,SpeedController*,void*);
    ~EnhancedRobotDrive();
    void doControls();
    static void disable(void*);
    static void shiftLowGear(void*);
    static void shiftHighGear(void*);
private:
    float drivePower; // 1.0 Normal and 0.6 Climbing
    Servo rightShifter;
    Servo leftShifter;
    robot_class::state* robotState;
    enum dir {
        LEFT,
        RIGHT
    };
    void swivle(dir);
    EnhancedJoystick* driver;
    EnhancedJoystick* gunner;
    static void update(void*);
};
#endif
