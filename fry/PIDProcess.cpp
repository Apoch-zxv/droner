//
// Created by grg on 12/09/15.
//

#include "PIDProcess.h"

#include "Arduino.h"
#include <string.h>

const unsigned long PIDProcess::DEFAULT_SAMPLE_TIME = 100;

PIDProcess::PIDProcess(const char* name, float desiredValue, float ki, float kd, float kp,
                       ControllerDirections controllerDirection, float outMin, float outMax)
        :desiredValue(desiredValue), specifiedDesiredValue(desiredValue), outMin(outMin), outMax(outMax),
         controllerDirection(controllerDirection), sampleTime(DEFAULT_SAMPLE_TIME), ITerm(0) {
    setTunings(kp, ki, kd);

    strcpy(this->name, name);
    this->lastExecMillis = millis() - this->sampleTime;
}

bool PIDProcess::compute(const float& currentValue) {
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
        float dInput = (currentValue - lastValue);

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

void PIDProcess::setTunings(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}