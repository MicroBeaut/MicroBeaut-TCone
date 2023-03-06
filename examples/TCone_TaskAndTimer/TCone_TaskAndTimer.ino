#include "TCone.h"

const uint8_t numberOfGroup = 3;
const uint8_t inputPins[] = {A3, A2, A1};
const uint8_t outputPins[] = {4, 3, 2};
bool inputStates[numberOfGroup];

const uint8_t numberOfTask = 2;
void inputTask();
void outputTask();

const TaskPointer taskPointer[] = {inputTask, outputTask};
const TimerTypes timerType[] = {timerON, timerOFF, timerON};

void setup() {
  Task.setScanTime(20);
  for (uint8_t index = 0; index < numberOfTask; index++) {
    Task.attachTask(taskPointer[index]);
  }

  for (uint8_t index = 0; index < numberOfGroup; index++) {
    pinMode(inputPins[index], INPUT_PULLUP);
    pinMode(outputPins[index], OUTPUT);
    Timer.timerEnable(true, index);
    Timer.timerType(timerType[index], index);
    Timer.timerDelay(2000, index);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  Task.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void inputTask() {
  for (uint8_t index = 0; index < numberOfGroup; index++) {
    inputStates[index] = !digitalRead(inputPins[index]);
    if (inputStates[index]) {
      Timer.timerStart(index);
    }
  }

}

void outputTask() {
  for (uint8_t index = 0; index < numberOfGroup; index++) {
    digitalWrite(outputPins[index], Timer.getOutput(index));
  }
}