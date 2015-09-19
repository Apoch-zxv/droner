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

    void setThrottle();

public:
    inline MotorController(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
            : a(a), b(b), c(c), d(d), athrottle(0), bthrottle(0), cthrottle(0), dthrottle(0) {};

    void init();

    void updateSpeed(const float& yawAction, const float& pitchAction, const float& rollAction);
};


#endif //DRONER_MOTORCONTROLLER_H
