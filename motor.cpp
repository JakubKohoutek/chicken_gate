#include <Stepper.h>

#include "Arduino.h"
#include "motor.h"
#include "melody.h"
#include "memory.h"

// 2036 is one full turn. Must be divisible by 4 as the motor is 4 step
#define ONE_TURN    2036

// Max reliable speed: 10
#define SPEED       8

// Number of steps required to fully open or close gate
#define STEPS_COUNT 8300

Stepper stepper = Stepper(ONE_TURN, STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);

void powerUpMotor () {
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  pinMode(MOTOR_POWER_PIN, OUTPUT);
  digitalWrite(MOTOR_POWER_PIN, HIGH);
  stepper.setSpeed(SPEED);
};

void powerDownMotor () {
  digitalWrite(MOTOR_POWER_PIN, LOW);
  pinMode(STEPPER_PIN_1, INPUT);
  pinMode(STEPPER_PIN_2, INPUT);
  pinMode(STEPPER_PIN_3, INPUT);
  pinMode(STEPPER_PIN_4, INPUT);
}

void openGate () {
  if(readFromMemory(STATE_ADDRESS) == OPEN) {
    return;
  }
  playMelody(melodyUp, melodyUpLength);
  powerUpMotor();
  int count = 0;
  while (count < STEPS_COUNT) {
    stepper.step(-1);
    count++;
    // NOTE: we must use yield, otherwise ESP will restart after some time
    yield();
  }
  powerDownMotor();
  writeToMemory(STATE_ADDRESS, OPEN);
};

void closeGate () {
  if(readFromMemory(STATE_ADDRESS) == CLOSE) {
    return;
  }
  playMelody(melodyDown, melodyDownLength);
  powerUpMotor();
  int count = 0;
  while (count < STEPS_COUNT) {
    stepper.step(1);
    count++;
    // NOTE: we must use yield, otherwise ESP will restart after some time
    yield();
  }
  powerDownMotor();
  writeToMemory(STATE_ADDRESS, CLOSE);
};