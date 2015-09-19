//
// Created by grg on 05/09/15.
//

#ifndef DRONER_QUTILS_H
#define DRONER_QUTILS_H

typedef unsigned char prog_uchar;

#define QDEBUG

#ifdef QDEBUG
    #define QDEBUG_LOG(x) Serial.print(x)
    #define QDEBUG_BASE(x, y) Serial.print(x, y)
    #define QDEBUG_BASELN(x) Serial.println(x)
    #define QDEBUG_BASELNF(x, y) Serial.println(x, y)
#else
    #define QDEBUG(x)
    #define QDEBUG_BASE(x, y)
    #define QDEBUG_BASELN(x)
    #define QDEBUG_BASELNF(x, y)
#endif

#endif //DRONER_QUTILS_H
