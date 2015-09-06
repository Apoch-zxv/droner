//
// Created by grg on 05/09/15.
//

#include "GyroWrapper.h"

volatile bool QGyroWrapper::MPU_INTERRUPT = false;

void dmpDataReady() {
    QGyroWrapper::MPU_INTERRUPT = true;
}

uint16_t QGyroWrapper::initialize() {
    QDEBUG_BASELN("Initializeing mpu");
    mpu.initialize();

    QDEBUG_BASELN("Testing connectivity");
    bool connection_test_res = mpu.testConnection();

    if (!connection_test_res) {
        QDEBUG_BASELN("Failed connecting to the MPU");
        return MPU_INIT_STATUS_MPU_ERROR;
    }

    QDEBUG_BASELN("Successfully connected to the MPU");
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again

    QDEBUG_BASELN("Initializing DNP");
    uint8_t dev_status = mpu.dmpInitialize();

    if (dev_status == 0) {
        QDEBUG_BASELN("Successfully initialized enabling");
        mpu.setDMPEnabled(true);
        QDEBUG_BASELN("setting interrupt");
        attachInterrupt(0, dmpDataReady, RISING);

        QDEBUG_BASELN("Clearing the interrupts");
        uint8_t mpuIntStatus = mpu.getIntStatus();
        packetSize = mpu.dmpGetFIFOPacketSize();

        return MPU_INIT_STATUS_SUCCESS;
    } else {
        QDEBUG_BASELN("Failed initializing the DMP");
        return MPU_INIT_STATUS_DNP_ERROR | dev_status;
    }
}