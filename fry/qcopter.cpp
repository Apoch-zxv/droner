#include "Arduino.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "qutils.h"
#include "external_libraries/I2Cdev.h"
#include "GyroWrapper.h"
#include "PIDProcess.h"
#include "MotorController.h"

QGyroWrapper gyro;
MotorController motorController(8, 9, 10, 11);
PIDProcess yawPidProcess(0, 0, 1, 0, NORMAL);
PIDProcess rollPidProcess(0, 0, 1, 0, NORMAL);
PIDProcess pitchPidProcess(0, 0, 1, 0, NORMAL);

#ifdef QDEBUG
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
#endif

#define LED_PIN 13
bool blinkState = false;

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

    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    //yawPidProcess.compute(gyro.getCurrentYaw());
    //rollPidProcess.compute(gyro.getCurrentRoll());
    //pitchPidProcess.compute(gyro.getCurrentPitch());
    //motorController.updateSpeed(yawPidProcess.getNextAction(), pitchPidProcess.getNextAction(),
    //                            rollPidProcess.getNextAction());

    if (gyro.loadLatestMeasurements()) {
#ifdef QDEBUG
        uint8_t * fifoBuffer = gyro.getFifoBuffer();
        teapotPacket[2] = fifoBuffer[0];
        teapotPacket[3] = fifoBuffer[1];
        teapotPacket[4] = fifoBuffer[4];
        teapotPacket[5] = fifoBuffer[5];
        teapotPacket[6] = fifoBuffer[8];
        teapotPacket[7] = fifoBuffer[9];
        teapotPacket[8] = fifoBuffer[12];
        teapotPacket[9] = fifoBuffer[13];
        Serial.write(teapotPacket, 14);
        teapotPacket[11]++; // packetCount, loops at 0xFF on purpose

        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
#endif
    }
}
