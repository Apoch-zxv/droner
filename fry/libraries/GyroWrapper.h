//
// Created by grg on 05/09/15.
//

#ifndef DRONER_GYROWRAPPER_H
#define DRONER_GYROWRAPPER_H

#include "qutils.h"
#include "Arduino.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define FIFO_BUFFER_SIZE 64

enum InitializationStatus {MPU_INIT_STATUS_SUCCESS = 0x10, MPU_INIT_STATUS_MPU_ERROR = 0x20, MPU_INIT_STATUS_DNP_ERROR = 0x40};

volatile bool MPU_INTERRUPT = false;

class QGyroWrapper {
private:
    MPU6050 mpu;
    uint16_t packetSize;
    uint8_t fifoBuffer[FIFO_BUFFER_SIZE];
public:
    uint16_t initialize();
    inline const uint16_t& getPacketSize() const { return packetSize; }
    inline volatile bool& getMpuInterrupt() { return MPU_INTERRUPT; }
};


#endif //DRONER_GYROWRAPPER_H
