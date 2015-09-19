#include "Arduino.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "qutils.h"
#include "I2Cdev.h"
#include "GyroWrapper.h"
#include "PIDProcess.h"

QGyroWrapper gyro;
MotorController motorController;
PIDProcess yawPidProcess(0, )

bool initMpu() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    #ifdef QDEBUG
        Serial.begin(115200);
        while (!Serial);
    #endif

    uint16_t res = gyro.initialize();

    if (res & MPU_INIT_STATUS_SUCCESS == 0) {
        QDEBUG_BASELN("Got an error response");
        QDEBUG_BASELNF(res, HEX);
        return false;
    } else {
        QDEBUG_BASELN("Successfully initialized");
        QDEBUG_BASELN(res);
        return true;
    }
}

bool initMotorController() {
    motorController.init();
    return true;
}

bool initMainBoardConnection() {
    // TODO
    return true;
}

void setup() {
    if (!initMpu()) {
        return;
    }

    if (!initMotorController()) {
        return;
    }

    if (!initMainBoardConnection()) {
        return;
    }
}

int count = 0;

void loop() {
    gyro.loadLatestMeasurements();

    if (count % 1000 == 0) {
        Serial.println(gyro.getCurrentYaw());
        Serial.println(gyro.getCurrentPitch());
        Serial.println(gyro.getCurrentRoll());
    }

    count ++;
}
