set(PROJECT_NAME fry)

set(CMAKE_ARDUINO_PATH ~/arduino-cmake-master)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_ARDUINO_PATH}/cmake/ArduinoToolchain.cmake) # Arduino Toolchain
set(CMAKE_EXTERNAL_LIBS_PATH ~/sketchbook/libraries) # arduino external libs


# Link external libs directories (as Arduino-IDE uses it)
link_directories(${CMAKE_EXTERNAL_LIBS_PATH})

cmake_minimum_required(VERSION 2.8)

project(${PROJECT_NAME} C CXX)

# find_package(Arduino 1.0 REQUIRED)

# Define your Arduino board (below is for Arduino uno)
set(${PROJECT_NAME}_BOARD uno)

# Define the source code
#set(${PROJECT_NAME}_SRCS qcopter.cpp external_libraries/I2Cdev.cpp external_libraries/MPU6050.cpp GyroWrapper.cpp MotorController.cpp PIDProcess.cpp DataPersistor.cpp)
set(${PROJECT_NAME}_SRCS qcopter.cpp external_libraries/I2Cdev.cpp external_libraries/MPU6050.cpp GyroWrapper.cpp MotorController.cpp PIDProcess.cpp)

# Define the port for uploading code to the Arduino
set(${PROJECT_NAME}_PORT /dev/ttyUSB0)
link_directories(/home/grg/work/droner/fry/build)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})

target_link_libraries(fry libuno_Servo.a)

