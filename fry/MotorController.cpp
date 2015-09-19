//
// Created by grg on 18/09/15.
//

#include "MotorController.h"

const unsigned int MotorController::SERVO_ATTACH_DELAY = 100;
const unsigned int MotorController::ESC_ARM_DELAY = 5000;
const unsigned int MotorController::ESC_MIN = 22;

void MotorController::init() {
    QDEBUG_BASELN("Initiating the motor controller");
    aserv.attach(a);
    bserv.attach(b);
    cserv.attach(c);
    dserv.attach(d);

    delay(SERVO_ATTACH_DELAY);

    QDEBUG_BASELN("Arming the escs");
    aserv.write(ESC_MIN);
    bserv.write(ESC_MIN);
    cserv.write(ESC_MIN);
    dserv.write(ESC_MIN);

    delay(ESC_ARM_DELAY);
}

void MotorController::updateSpeed(const float& yawAction, const float& pitchAction, const float& rollAction) {
    athrottle += rollAction - yawAction;
    bthrottle += pitchAction + yawAction;
    cthrottle += - rollAction - yawAction;
    dthrottle += - pitchAction + yawAction;

    setThrottle();
}

void MotorController::setThrottle() {
    aserv.write(athrottle);
    bserv.write(bthrottle);
    cserv.write(cthrottle);
    dserv.write(dthrottle);
}