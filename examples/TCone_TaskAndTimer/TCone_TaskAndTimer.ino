
/*
  TCone_TaskAndTimer.ino
  Created:  7-Mar-2023
  Author:   MicroBeaut
*/

/*
  MicroBeaut TCone Multitask and Timer ON/OFF control
*/

#include "TCone.h"

const uint8_t numberOfGroup = 3;            // Number of group
const uint8_t inputPins[] = {A3, A2, A1};   // Array of input pins
const uint8_t outputPins[] = {4, 3, 2};     // Array of output pins
bool inputStates[numberOfGroup];            // Array of input state

const uint8_t numberOfTask = 2;             // Number of Task
void inputTask();                           // Delclare subroutine
void outputTask();                          // Delclare subroutine

const TaskPointer taskPointer[] = {inputTask, outputTask};    // Pointer Array of subroutine
const TimerTypes timerType[] = {TON, TOF, TON};               // Timer Type Array
const uint16_t timerDelay[] = {1000, 2000, 3000};             // Timer delay

void setup() {
  Task.setScanTime(20);                                       // Set scan time or Timer interrupt. Interval 20 milliseconds
  for (uint8_t index = 0; index < numberOfTask; index++) {
    Task.attachTask(taskPointer[index]);                      // Attached Task
  }

  for (uint8_t index = 0; index < numberOfGroup; index++) {
    pinMode(inputPins[index], INPUT_PULLUP);                  // Set pin mode
    pinMode(outputPins[index], OUTPUT);                       // Set pin mode
    Timer.timerEnable(true, index);                           // Enable Timer
    Timer.timerType(timerType[index], index);                 // Set type of timer
    Timer.timerDelay(timerDelay[index], index);               // Set time delay
  }
  pinMode(LED_BUILTIN, OUTPUT);                               // Set pin mode
  Task.begin();                                               // Task begin
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);                            // Turn LED ON
  delay(1000);                                                // Delay for 1 second
  digitalWrite(LED_BUILTIN, LOW);                             // Turn LED OFF
  delay(1000);                                                // Delay for 1 second
}

void inputTask() {
  for (uint8_t index = 0; index < numberOfGroup; index++) {
    inputStates[index] = !digitalRead(inputPins[index]);      // Read button state
    if (inputStates[index]) {                                 // Compare button state
      Timer.timerStart(index);                                // Start Timer
    }
  }
}

void outputTask() {
  for (uint8_t index = 0; index < numberOfGroup; index++) {
    digitalWrite(outputPins[index], Timer.getOutput(index));  // Write timer output ot LED
  }
}