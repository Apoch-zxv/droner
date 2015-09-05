#define DEBUG

#include "Arduino.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include "qutils.h"
#include "I2Cdev.h"
#include "GyroWrapper.h"

QGyroWrapper gyro;

void setup() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(115200);
    while (!Serial);

    uint16_t res = gyro.initialize();

    if (res & MPU_INIT_STATUS_SUCCESS == 0) {
        QDEBUG_BASELN("Got an error response");
        QDEBUG_BASELNF(res, HEX);
    }

    QDEBUG_BASELN("Successfully initialized");
}

void loop() {

}
