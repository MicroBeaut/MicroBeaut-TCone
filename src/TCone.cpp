/*
  Task.cpp
  Created:  4-Mar-2023
  Author:   MicroBeaut
*/

/*
  MIT License
  Copyright (c) 2023 MicroBeaut
*/
#include "TCone.h"


TConeTask Task;
TConeTimer Timer;

#if defined(ARDUINO_ARCH_AVR) // ARDUINO_ARCH_AVR - TCON-TASK

TConeTask::TConeTask()
{
  for (uint8_t index = 0; index < TCONE_MAX_TASK; index++) {
    _taskPointer[index] = nullptr;
  }
  _taskCount = 0;
  _scanTime = TCONE_DEFAULT_SCANTIME;
}

void TConeTask::attachTask(TaskPointer taskName) {
  if (_taskCount >= TCONE_MAX_TASK)  return;
  _taskPointer[_taskCount++] = taskName;
}

void TConeTask::setScanTime(uint16_t scanTime) {
  _scanTime = scanTime > TCONE_MAX_SCAN_TIME ? TCONE_MAX_SCAN_TIME : scanTime;
  _scanTime = scanTime < TCONE_MIN_SCAN_TIME ? TCONE_MIN_SCAN_TIME : scanTime;
}

void TConeTask::begin() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1A = (1 << COM1A0);
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  OCR1A = MS2OCR( _scanTime);
  TIMSK1 |= (1 << OCIE1A);
}

void TConeTask::taskRun() {
  for (uint8_t index = 0; index < _taskCount; index ++) {
    if (_taskPointer[index]) {
      _taskPointer[index]();
    }
  }
}

uint16_t TConeTask::getScanTime() {
  return _scanTime;
}

uint8_t TConeTask::countTask() {
  return _taskCount;
}

#endif // ARDUINO_ARCH_AVR - TCON-TASK

#if defined(ARDUINO_ARCH_AVR) // ARDUINO_ARCH_AVR - TCON-TIMER

TConeTimer::TConeTimer()
{
  this->internalSetTimerAll(false, 0, false, false);
  internalSetEnableAll(false);
  _timerCount = TCONE_MAX_TIMER;
}


void TConeTimer::timerMonitor() {
  for (uint8_t index = 0; index < _timerCount; index ++) {
    this->internalTimerMonitor(index);
  }
}

void TConeTimer::internalTimerMonitor(uint8_t index) {
  if (!_timer[index].enable) return;
  if (!_timer[index].start) return;
  if (_timer[index].done) return;
  _timer[index].elapsedTime += Task.getScanTime() ;
  if (_timer[index].elapsedTime >= _timer[index].timeDelay) {
    this->internalTimerDone(index);
  }
}

void TConeTimer::timerType(TimerTypes type, uint8_t index) {
  if (index >= _timerCount) return;
  _timer[index].type = type;
  this->timerStop(index);
}

void TConeTimer::timerDelay(uint16_t timeDelay, uint8_t index) {
  if (index >= _timerCount) return;
  _timer[index].timeDelay = timeDelay > TCONE_MAX_DELAY_TIME ? TCONE_MAX_DELAY_TIME : timeDelay;
  _timer[index].timeDelay = timeDelay < TCONE_MIN_DELAY_TIME ? TCONE_MIN_DELAY_TIME : timeDelay;
}

void TConeTimer::timerStart(uint8_t index) {
  if (index >= _timerCount) return;
  if (!_timer[index].enable) return;
  bool output = _timer[index].type == TON ? false : true;
  this->internalSetTimer(true, 0, output, false, index);
}

void TConeTimer::timerStop(uint8_t index) {
  if (index >= _timerCount) return;
  if (!_timer[index].enable) return;
  bool output = _timer[index].type == TON ? false : true;
  this->internalSetTimer(false, 0, output, false, index);
}

void TConeTimer::internalStartStop(bool start, uint8_t index) {
  if (index >= _timerCount) return;
  if (!_timer[index].enable) return;
  bool output = _timer[index].type == TON ? false : true;
  this->internalSetTimer(start, 0, output, false, index);
}

void TConeTimer::internalSetEnableAll(bool enable) {
  for (uint8_t index = 0; index < _timerCount; index++) {
    this->timerEnable(enable, index);
  }
}

void TConeTimer::timerEnable(bool enable, uint8_t index) {
  if (index >= _timerCount) return;
  _timer[index].enable = enable;
  this->timerStop(index);
}

void TConeTimer::internalTimerDone(uint8_t index) {
  if (!_timer[index].enable) return;
  bool output = _timer[index].type == TON ? true : false;
  this->internalSetTimer(true, 0, output, true, index);
}

void TConeTimer::internalSetTimerAll(bool start, uint16_t elapsedTime, bool output, bool done) {
  for (uint8_t index = 0; index < _timerCount; index++) {
    this->internalSetTimer(start, elapsedTime, output, done, index);
  }
}

void TConeTimer::internalSetTimer(bool start, uint16_t elapsedTime, bool output, bool done, uint8_t index) {
  if (!_timer[index].enable) return;
  _timer[index].start = start;
  _timer[index].elapsedTime = elapsedTime;
  _timer[index].output = output;
  _timer[index].done = done;
}

bool TConeTimer::getOutput(uint8_t index) {
  if (index >= _timerCount) return false;
  return _timer[index].output;
}

bool TConeTimer::isDone(uint8_t index) {
  if (index >= _timerCount) return false;
  return _timer[index].done;
}

uint8_t TConeTimer::getTimerCount() {
  return _timerCount;
}

ISR (TIMER1_COMPA_vect) {
  Task.taskRun();
  Timer.timerMonitor();
}
#endif // ARDUINO_ARCH_AVR - TCON-TIMER