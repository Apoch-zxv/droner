//
// Created by grg on 28/09/15.
//
#include "Arduino.h"

#include "DataPersistor.h"
#include "qutils.h"

#include "EEPROM/EEPROM.h"

void DataPersistor::initialize() {
    readBuffer((uint8_t *) (&globalHeader), 0, sizeof(GlobalHeader));

    if (globalHeader.version != CURRENT_VERSION) {
        QDEBUG_BASELN("Invalid version clearning all");
        clearAll();
        dumpHeader();
        loadChannelsHeaders(&DataPersistor::emptyStaticChannelsHeadersInit);
    } else {
        QDEBUG_BASELN("Same version loading all the data");
        loadChannelsHeaders(&DataPersistor::persistentStaticChannelsHeadersInit);
    }
}

void DataPersistor::dumpHeader() {
    writeBuffer((uint8_t*) &globalHeader, 0, sizeof(GlobalHeader));
}

void DataPersistor::clearAll() {
    for (int i = 0; i < MEMORY_SIZE; i ++) {
        EEPROM.write(i, 0);
    }
}

void DataPersistor::persistentStaticChannelsHeadersInit(StaticChannelHeader* staticChannelsHeaders) {
    uint16_t curOffset = sizeof(GlobalHeader);
    for (int i = 0; i < CHANNELS_NUMBER; i ++) {
        staticChannelsHeaders[i].lastWrite = EEPROM.read(curOffset);
        curOffset += channelDefinitions[i].blockInChannel + channelDefinitions[i].baseBlockSize;
    }
}

void DataPersistor::emptyStaticChannelsHeadersInit(StaticChannelHeader* staticChannelsHeaders) {
    for (int i = 0; i < CHANNELS_NUMBER; i ++) {
        staticChannelsHeaders[i].lastWrite = 0;
    }
}

void DataPersistor::loadChannelsHeaders(void (*staticChannelsHeadersInit)(StaticChannelHeader* staticChannelsHeaders)) {
    StaticChannelHeader staticChannelsHeaders[CHANNELS_NUMBER];
    staticChannelsHeadersInit(staticChannelsHeaders);

    uint16_t curOffset = sizeof(GlobalHeader);
    for (int i = 0; i < CHANNELS_NUMBER; i ++) {
        channelHeaders[i].channelDefinition = channelDefinitions[i];
        channelHeaders[i].staticChannelHeader = staticChannelsHeaders[i];
        channelHeaders[i].startOffset = curOffset + sizeof(StaticChannelHeader);

        curOffset += channelHeaders[i].channelDefinition.baseBlockSize * channelHeaders[i].channelDefinition.blockInChannel;
        channelHeaders[i].endOffset = curOffset;
    }
}

void DataPersistor::writeBuffer(uint8_t* buf, unsigned int startIndex, unsigned int size) {
    for (int i = startIndex; i < (startIndex + size); i ++) {
        EEPROM.write(i, buf[i - startIndex]);
    }
}

void DataPersistor::readBuffer(uint8_t* buf, unsigned int startIndex, unsigned int size) {
    for (int i = startIndex; i < (startIndex + size); i ++) {
        buf[i - startIndex] = EEPROM.read(i);
    }
}

uint16_t DataPersistor::getNextIndex(uint16_t curIndex, uint8_t channelIndex) {
    DynamicChannelHeader& channelHeader = channelHeaders[channelIndex];

    uint16_t rawNextIndex = curIndex + channelHeader.channelDefinition.baseBlockSize;
    if (rawNextIndex > channelHeader.endOffset) {
        return channelHeader.startOffset;
    }

    return rawNextIndex;
}

uint16_t DataPersistor::getNextIndex(uint8_t channelIndex) {
    DynamicChannelHeader& channelHeader = channelHeaders[channelIndex];
    return getNextIndex(channelHeader.staticChannelHeader.lastWrite, channelIndex);
}

void DataPersistor::writeData(uint8_t* buf, uint8_t channelIndex) {
    DynamicChannelHeader& channelHeader = channelHeaders[channelIndex];
    uint16_t nextIndex = getNextIndex(channelIndex);
    bool isOverflow = nextIndex < channelHeader.staticChannelHeader.lastWrite;

    if (isOverflow && !channelHeader.channelDefinition.isOverwrite) {
        return;
    }

    writeBuffer(buf, nextIndex, channelHeader.channelDefinition.baseBlockSize);
    channelHeader.staticChannelHeader.lastWrite = nextIndex;
}

template<typename T> T* DataPersistor::readChannelsData(uint8_t channelIndex) {
    DynamicChannelHeader& channelHeader = channelHeaders[channelIndex];
    T* res = new T[channelHeader.channelDefinition.blockInChannel];
    uint16_t firstIndex = getNextIndex(channelIndex);
    uint16_t currentIndex = firstIndex;
    int counter = 0;

    do {
        readBuffer(&res[counter], currentIndex, channelHeader.channelDefinition.baseBlockSize);

        currentIndex = getNextIndex(currentIndex, channelIndex);
        counter ++;
    } while (currentIndex != firstIndex);
}