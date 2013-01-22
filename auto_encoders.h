#ifndef AUTO_ENCODERS
#define AUTO_ENCODERS

#include <Encoder.h>
#include "ports.h"

class auto_encoders {
private:
    double distance;
    Encoder* left_encoder;
    Encoder* right_encoder;
    left_encoder.SetDistancePerPulse(0.20106192983);
    right_encoder.SetDistancePerPulse(0.20106192983);
    double get_avg_ticks();
    double convert_distance(double);
    static const double WHEEL_RADIUS = 4.0;
    //static const double WHEEL_CIRCUMFERENCE = WHEEL_RADIUS * 2 * pi;
    static const double TICKS_PER_REV = 250; // PULSES PER REV = 125, DISTANCE PER PULSE = circumference / 125
    static const double DRIVE_REDUCTION = 38.0/60;
    static const double pi = 3.14159265358979;
public:
    ~auto_encoders();
    auto_encoders(hw_info, hw_info, hw_info, hw_info);
    double get_target_distance();
    double get_distance();
    void reset_distance();
    void start_driving(double);
    void stop_driving();
};

#endif
