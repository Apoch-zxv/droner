//
// Created by grg on 12/09/15.
//

#ifndef DRONER_PIDPROCESS_H
#define DRONER_PIDPROCESS_H

enum ControllerDirections {NORMAL, REVERSE};

class PIDProcess {
private:
    float desiredValue;
    float lastValue;
    float ki, kd, kp;
    float nextAction;
    float ITerm;
    float outMin, outMax;
    unsigned long sampleTime, lastExecMillis;
    ControllerDirections controllerDirection;

public:
    PIDProcess(float desiredValue, float ki, float kd, float kp,
               ControllerDirections controllerDirection, float outMin = 0, float outMax = 255);
    bool compute(const float& currentValue);

    void setTunings(float kp, float ki, float kd);
    const float& getNextAction() const { return nextAction; }
    void setDesiredValue(const float& desiredValue) { this->desiredValue = desiredValue; }
};


#endif //DRONER_PIDPROCESS_H
