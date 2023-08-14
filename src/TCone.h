/*
  Task.h
  Created:  4-Mar-2023
  Author:   MicroBeaut
*/

/*
  MIT License
  Copyright (c) 2023 MicroBeaut
*/

#ifndef TCONE_H
#define TCONE_H

#include "Arduino.h"

#if defined(ARDUINO_ARCH_AVR)

enum TimerTypes {
  TON = 0,
  TOF = 1
};

typedef struct {
  unsigned char enable: 1;
  unsigned char start: 1;
  unsigned char output: 1;
  unsigned char done: 1;
  uint16_t elapsedTime = 0;
  uint16_t timeDelay = 1000;  // Default 1000 milliseconds
  TimerTypes type = TON;
} TimerObject;



#define MS2US(ms) (ms*1000UL)

const uint8_t MIN_CLOCK_SELECT = 0;     // Minimum Clock Select Mode
const uint8_t MAX_CLOCK_SELECT = 6;     // Maximum Clock Select Mode
const uint8_t DEFAULT_CLOCK_SELECT = 5; // Default Clock Select Mode

const uint16_t TCONE_MIN_SCAN_TIME = 10;  // 10   milliseconds
const uint16_t TCONE_MAX_SCAN_TIME = 250; // 250  milliseconds

const uint16_t TCONE_MIN_DELAY_TIME = 10;     // 10    milliseconds
const uint16_t TCONE_MAX_DELAY_TIME = 60000;  // 60000 milliseconds = 60 second

const uint8_t TCONE_MAX_TIMER  = 12;
const uint8_t TCONE_MAX_TASK  = 12;
const uint16_t TCONE_DEFAULT_SCANTIME = 10;

typedef void (*TaskPointer)(void);

/// @brief Prescaler Divider
const uint16_t prescalerDivider [] = {0,
                                      1,
                                      8,
                                      64,
                                      256,
                                      1024
                                     };

/// @brief Timer Clock Frequency
const unsigned long timerFrequency[] = {0UL,
                                        (unsigned long)F_CPU / prescalerDivider[1],
                                        (unsigned long)F_CPU / prescalerDivider[2],
                                        (unsigned long)F_CPU / prescalerDivider[3],
                                        (unsigned long)F_CPU / prescalerDivider[4],
                                        (unsigned long)F_CPU / prescalerDivider[5]
                                       };
// /// @brief Timer Time
// const float timerTime[] = {0,
//                            1000000.0f / timerFrequency[1],
//                            1000000.0f / timerFrequency[2],
//                            1000000.0f / timerFrequency[3],
//                            1000000.0f / timerFrequency[4],
//                            1000000.0f / timerFrequency[5]
//                           };

#define MS2OCR(ms) ((timerFrequency[DEFAULT_CLOCK_SELECT] * ms / 1000UL) + 1)
#else
#error The TCone library only supports Arduino AVR Family
#endif // ARDUINO_ARCH_AVR - CONSTANT

class TConeTask {
#if defined(ARDUINO_ARCH_AVR)
  private:
    volatile TaskPointer _taskPointer[TCONE_MAX_TASK];
    volatile uint8_t _taskCount = 0;
    volatile uint16_t _scanTime = 0;
#endif // ARDUINO_ARCH_AVR
  public:
    TConeTask();
#if defined(ARDUINO_ARCH_AVR)
    void taskRun();
    void attachTask(TaskPointer taskName);
    void setScanTime(uint16_t setScanTime);
    void begin();
    uint16_t getScanTime();
    uint8_t countTask();

#endif // ARDUINO_ARCH_AVR
};

class TConeTimer {
#if defined(ARDUINO_ARCH_AVR)
  private:
    volatile TimerObject _timer[TCONE_MAX_TIMER];
    volatile uint8_t _timerCount = 0;

    void internalSetEnableAll(bool enable);
    void internalTimerDone(uint8_t index);
    void internalSetTimerAll(bool start, uint16_t elapsedTime, bool output, bool done);
    void internalSetTimer(bool start, uint16_t elapsedTimer, bool output, bool done, uint8_t index);
    void internalTimerMonitor(uint8_t index);
    void internalStartStop(bool start, uint8_t index);
#endif // ARDUINO_ARCH_AVR
  public:
    TConeTimer();
#if defined(ARDUINO_ARCH_AVR)
    void timerMonitor();
    void timerEnable(bool enable, uint8_t index);
    void timerStart(uint8_t index);
    void timerStop(uint8_t index);
    void timerType(TimerTypes type, uint8_t index);
    void timerDelay(uint16_t timeDelay, uint8_t index);
    bool getOutput(uint8_t index);
    bool isDone(uint8_t index);
    uint8_t getTimerCount();


#endif // ARDUINO_ARCH_AVR
};

extern TConeTask Task; //dfa
extern TConeTimer Timer;

#endif // TCONE_H