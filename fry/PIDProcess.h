//
// Created by grg on 12/09/15.
//

#ifndef DRONER_PIDPROCESS_H
#define DRONER_PIDPROCESS_H

enum ControllerDirections {NORMAL, REVERSE};

class PIDProcess {
private:
    static const unsigned long DEFAULT_SAMPLE_TIME;

    char name[100];
    float desiredValue;
    float specifiedDesiredValue;
    float lastValue;
    float ki, kd, kp;
    float nextAction;
    float ITerm;
    float outMin, outMax;
    unsigned long sampleTime, lastExecMillis;
    ControllerDirections controllerDirection;

public:
    PIDProcess(const char * name, float desiredValue, float ki, float kd, float kp,
               ControllerDirections controllerDirection, float outMin = -100., float outMax = 100.);
    bool compute(const float& currentValue);

    inline void normalDesiredValue(const float& currentValue) {
        desiredValue = currentValue + specifiedDesiredValue;
    };
    inline const float& getDesiredValue() { return desiredValue; };

    void setTunings(float kp, float ki, float kd);
    const float& getNextAction() const { return nextAction; }
    void setDesiredValue(const float& desiredValue) { this->desiredValue = desiredValue; }
};


#endif //DRONER_PIDPROCESS_H
