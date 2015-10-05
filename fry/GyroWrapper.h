//
// Created by grg on 05/09/15.
//

#ifndef DRONER_GYROWRAPPER_H
#define DRONER_GYROWRAPPER_H

#include "qutils.h"
#include "Arduino.h"
#include "external_libraries/MPU6050_6Axis_MotionApps20.h"

#define FIFO_BUFFER_SIZE 64

enum InitializationStatus {MPU_INIT_STATUS_SUCCESS = 0x10, MPU_INIT_STATUS_MPU_ERROR = 0x20, MPU_INIT_STATUS_DNP_ERROR = 0x40};

#define YAW_INDEX 0
#define PITCH_INDEX 2
#define ROLL_INDEX 1
#define AXIS_NUM 3

class QGyroWrapper {
private:
    MPU6050 mpu;
    Quaternion currentQuaternion;
    VectorFloat currentGravity;
    float currentYPR[AXIS_NUM];
    float currentYaw, currentPitch, currentRoll;

    uint16_t packetSize;
    uint8_t lastInterruptState;
    uint8_t fifoBuffer[FIFO_BUFFER_SIZE];

public:
    static volatile bool MPU_INTERRUPT;

    uint16_t initialize();
    bool loadLatestMeasurements();
    inline const uint16_t& getPacketSize() const { return packetSize; }
    inline volatile bool& getMpuInterrupt() { return MPU_INTERRUPT; }

    inline const float& getCurrentYaw() const { return currentYaw; }
    inline const float& getCurrentPitch() const { return currentPitch; }
    inline const float& getCurrentRoll() const { return currentRoll; }

    inline uint8_t* getFifoBuffer() { return fifoBuffer; }
};


#endif //DRONER_GYROWRAPPER_H
