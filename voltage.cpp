#include "Arduino.h"
#include "voltage.h"

float readVoltage () {
  int sensorValue = analogRead(A0);
  float voltage = (float)map(sensorValue, 13, 796, 0, 840) / 100;

  return voltage;
}
