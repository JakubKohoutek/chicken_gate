#ifndef MOTOR_H
#define MOTOR_H

#define MOTOR_POWER_PIN       0
#define STEPPER_PIN_1         1
#define STEPPER_PIN_2         3
#define STEPPER_PIN_3         12
#define STEPPER_PIN_4         16

void powerUpMotor ();

void powerDownMotor ();

void openGate ();

void closeGate ();

#endif // MOTOR_H