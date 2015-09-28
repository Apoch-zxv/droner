//
// Created by grg on 18/09/15.
//

#include "MotorController.h"

const unsigned int MotorController::SERVO_ATTACH_DELAY = 100;
const unsigned int MotorController::ESC_ARM_DELAY = 5000;
const unsigned int MotorController::ESC_MIN = 700;
const unsigned int MotorController::ESC_MAX = 2000;

void MotorController::init() {
    QDEBUG_BASELN("Initiating the motor controller");
    aserv.attach(a);
    bserv.attach(b);
    cserv.attach(c);
    dserv.attach(d);

    delay(SERVO_ATTACH_DELAY);

    QDEBUG_BASELN("Arming the escs 5 sec delay....");
    QDEBUG_BASELN(ESC_MIN);

    aserv.write(ESC_MIN);
    bserv.write(ESC_MIN);
    cserv.write(ESC_MIN);
    dserv.write(ESC_MIN);

    delay(ESC_ARM_DELAY);
}

unsigned int a = 0;

void MotorController::validateInRange(unsigned int* value) {
    if ((*value) < minimal_speed) {
        (*value) = minimal_speed;
    }
    if ((*value) > ESC_MAX) {
        (*value) = ESC_MAX;
    }
}

void MotorController::updateSpeed(const float& yawAction, const float& pitchAction, const float& rollAction, unsigned int total_speed) {
    athrottle = (int) athrottle + rollAction - yawAction + total_speed;
    bthrottle = (int) bthrottle + pitchAction + yawAction + total_speed;
    cthrottle = (int) cthrottle - rollAction - yawAction + total_speed;
    dthrottle = (int) dthrottle - pitchAction + yawAction + total_speed;

    validateInRange(&athrottle);
    validateInRange(&bthrottle);
    validateInRange(&cthrottle);
    validateInRange(&dthrottle);

    setThrottle();
}

void MotorController::forceStop() {
    athrottle = ESC_MIN;
    bthrottle = ESC_MIN;
    cthrottle = ESC_MIN;
    dthrottle = ESC_MIN;

    setThrottle();
}

void MotorController::setThrottle() {
    /*if (a % 10000 == 0) {
        Serial.println(athrottle);
        Serial.println(bthrottle);
        Serial.println(cthrottle);
        Serial.println(dthrottle);
    }
    a ++;*/

    aserv.write(athrottle);
    bserv.write(bthrottle);
    cserv.write(cthrottle);
    dserv.write(dthrottle);
}