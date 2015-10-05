//
// Created by grg on 28/09/15.
//

#ifndef DRONER_DATAPERSISTOR_H
#define DRONER_DATAPERSISTOR_H

struct StaticChannelHeader {
    uint16_t lastWrite;
};

struct ChannelDefinition {
    bool isOverwrite;
    int firstIndexToWriteAfter;
    int lastIndexToWriteAfter;
    uint8_t baseBlockSize;
    uint8_t blockInChannel;
};

struct DynamicChannelHeader {
    StaticChannelHeader staticChannelHeader;
    ChannelDefinition channelDefinition;
    uint16_t startOffset;
    uint16_t endOffset;
};

struct GlobalHeader {
    uint8_t version;
};

#define CHANNELS_NUMBER 1

class DataPersistor {
private:
    static const uint8_t  CURRENT_VERSION = 1;
    static DynamicChannelHeader channelHeaders[CHANNELS_NUMBER];
    static GlobalHeader globalHeader;
    static void readBuffer(uint8_t* buf, unsigned int startIndex, unsigned int size);
    static void writeBuffer(uint8_t* buf, unsigned int startIndex, unsigned int size);
    static void dumpHeader();
    static void clearAll();
    static void loadChannelsHeaders(void (*staticChannelsHeadersInit)(StaticChannelHeader* staticChannelsHeaders));

    static void emptyStaticChannelsHeadersInit(StaticChannelHeader* staticChannelsHeaders);
    static void persistentStaticChannelsHeadersInit(StaticChannelHeader* staticChannelsHeaders);

    static uint16_t getNextIndex(uint16_t curIndex, uint8_t channelIndex);
    static uint16_t getNextIndex(uint8_t channelIndex);

public:
    static const unsigned int MEMORY_SIZE = 1024;
    static const unsigned int DATA_MEMORY_SIZE = DataPersistor::MEMORY_SIZE - sizeof(GlobalHeader) - sizeof(StaticChannelHeader) * CHANNELS_NUMBER;

    static void initialize();
    static void writeData(uint8_t * buf, uint8_t channelIndex);
    template<typename T> static T* readChannelsData(uint8_t channelIndex);
};

struct DebugData {
    uint8_t yaw;
    uint8_t pitch;
    uint8_t roll;

    uint8_t yawAction;
    uint8_t pitchAction;
    uint8_t rollAction;
};

ChannelDefinition channelDefinitions[] = {{false, 0, -1, sizeof(DebugData), DataPersistor::DATA_MEMORY_SIZE / sizeof(DebugData)}};


#endif //DRONER_DATAPERSISTOR_H
