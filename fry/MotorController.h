//
// Created by grg on 18/09/15.
//

#ifndef DRONER_MOTORCONTROLLER_H
#define DRONER_MOTORCONTROLLER_H

#include "qutils.h"
#include "Arduino.h"
#include <Servo.h>

class MotorController {
private:
    static const unsigned int SERVO_ATTACH_DELAY;
    static const unsigned int ESC_ARM_DELAY;
    static const unsigned int ESC_MIN;
    static const unsigned int ESC_MAX;

    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;

    unsigned int athrottle;
    unsigned int bthrottle;
    unsigned int cthrottle;
    unsigned int dthrottle;

    Servo aserv;
    Servo bserv;
    Servo cserv;
    Servo dserv;

    unsigned int minimal_speed;

    void setThrottle();

    void validateInRange(unsigned int* value);

public:
    inline MotorController(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int minimal_speed)
            : a(a), b(b), c(c), d(d), athrottle(minimal_speed), bthrottle(minimal_speed), cthrottle(minimal_speed), dthrottle(minimal_speed), minimal_speed(minimal_speed) {};

    void init();

    void forceStop();

    void updateSpeed(const float& yawAction, const float& pitchAction, const float& rollAction, unsigned int total_speed);
};


#endif //DRONER_MOTORCONTROLLER_H
