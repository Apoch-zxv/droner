#include "Arduino.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "qutils.h"
#include "external_libraries/I2Cdev.h"
#include "GyroWrapper.h"
#include "PIDProcess.h"
#include "MotorController.h"
//#include "DataPersistor.h"

QGyroWrapper gyro;
MotorController motorController(10, 11, 12, 13, 1350);
PIDProcess yawPidProcess("yaw", 0, 0, 0, 0.001, NORMAL);
PIDProcess rollPidProcess("roll", 0, 0, 0, 0.001, NORMAL);
PIDProcess pitchPidProcess("pitch", 0, 0, 0, 0.001, NORMAL);
unsigned long time = millis();
bool isPrintedStopping = false;
bool isLearningMode = true;

#ifdef DISPLAY
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
#endif

#define LED_PIN 13
bool blinkState = false;

long readVcc() {
    long result;
    // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA,ADSC));
    result = ADCL;
    result |= ADCH<<8;
    result = 1126400L / result; // Back-calculate AVcc in mV
    return result;
}

bool isConnectedToPC() {
    return readVcc() == 5166;
}

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
    if (!initMotorController()) {
        QDEBUG_BASELN("Failed initiating the motor controller");
        return;
    }

    if (!initMpu()) {
        QDEBUG_BASELN("Failed initiating the MPU");
        return;
    }

    if (!initMainBoardConnection()) {
        QDEBUG_BASELN("Failed initiating the main board");
        return;
    }

//    DataPersistor::initialize();

    QDEBUG_BASELN("Initialization completed successfully");
}

void print_displayable() {

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
}

int counter = 0;

void loop() {
    if (millis() - time > 13000) {
        motorController.forceStop();
        if (!isPrintedStopping) {
            QDEBUG_BASELN("Stopping");
            isPrintedStopping = true;
        }
        return;
    }

    if (millis() - time > 10000) {
        isLearningMode = false;
    }

    gyro.loadLatestMeasurements();

    if (isLearningMode) {
        // The gyro scope will not be exactly flat so we remember the first values it received
        yawPidProcess.normalDesiredValue(gyro.getCurrentYaw());
        rollPidProcess.normalDesiredValue(gyro.getCurrentRoll());
        pitchPidProcess.normalDesiredValue(gyro.getCurrentPitch());
    } else {
        yawPidProcess.compute(gyro.getCurrentYaw());
        rollPidProcess.compute(gyro.getCurrentRoll());
        pitchPidProcess.compute(gyro.getCurrentPitch());
        motorController.updateSpeed(0, pitchPidProcess.getNextAction(),
                                    rollPidProcess.getNextAction(), 0);
    }

//    if (counter % 2000 == 0) {
//        Serial.println("rol");
//        Serial.println(rollPidProcess.getDesiredValue() - gyro.getCurrentRoll());
//        Serial.println(rollPidProcess.getNextAction());
//        Serial.println("rol");
//        Serial.println("yaw");
//        Serial.println(yawPidProcess.getDesiredValue() - gyro.getCurrentYaw());
//        Serial.println(yawPidProcess.getNextAction());
//        Serial.println("yaw");
//        Serial.println("pitch");
//        Serial.println(pitchPidProcess.getDesiredValue() - gyro.getCurrentPitch());
//        Serial.println(pitchPidProcess.getNextAction());
//        Serial.println("pitch");
//        motorController.printThrottle();
//    }

    counter ++;

//    DebugData d = {((uint8_t) gyro.getCurrentYaw() * 10), ((uint8_t) gyro.getCurrentPitch() * 10), ((uint8_t) gyro.getCurrentRoll() * 10),
//                   ((uint8_t) yawPidProcess.getNextAction()), ((uint8_t) pitchPidProcess.getNextAction()), ((uint8_t) rollPidProcess.getNextAction())};
//    DataPersistor::writeData((uint8_t*)&d, 0);
}
