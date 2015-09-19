//
// Created by grg on 12/09/15.
//

#include "PIDProcess.h"


PIDProcess::PIDProcess(float desiredValue, float ki, float kd, float kp,
                       ControllerDirections controllerDirection, float outMin = 0, float outMax = 255)
        :desiredValue(desiredValue), outMin(outMin), outMax(outMax),
         controllerDirection(controllerDirection), sampleTime(DEFAULT_SAMPLE_TIME), ITerm(0) {
    setTunnings(kp, ki, kd);

    this->lastExecMillis = millis() - this->sampleTime;
}

const float& PIDProcess::compute(const float& currentValue) {
    unsigned long now = millis();
    unsigned long timeDiff = (now - lastExecMillis);

    if(timeDiff >= sampleTime)
    {
        /*Compute all the working error variables*/
        float error = desiredValue - currentValue;
        ITerm += (ki * error);
        if(ITerm > outMax) {
            ITerm = outMax;
        }
        else if(ITerm < outMin) {
            ITerm= outMin;
        }
        float dInput = (currentValue - lastInput);

        /*Compute PID Output*/
        float output = kp * error + ITerm- kd * dInput;

        if(output > outMax) {
            output = outMax;
        }
        else if(output < outMin) {
            output = outMin;
        }
        nextAction = output;

        /*Remember some variables for next time*/
        lastValue = currentValue;
        lastExecMillis = now;
        return true;
    }
    else {
        return false;
    }
}

void PID::setTunings(float kp, float ki, float kd) {
    float sampleTimeSec = ((float)sampleTime)/1000;
    this->kp = kp;
    this->ki = ki * sampleTimeSec;
    this->kd = kd / sampleTimeSec;

    if(controllerDirection == REVERSE)
    {
        this->kp = (0 - this->kp);
        this->ki = (0 - this->ki);
        this->kd = (0 - this->kd);
    }
}