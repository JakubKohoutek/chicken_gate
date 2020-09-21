#include <Servo.h>

#include "config.h"
#include "melody.h"
#include "rtc.h"
#include "server.h"

#define LED_PIN          2
#define SLEEP_TIME       3*60*60e6 // the highest sleep period that works reliably
#define BUZZER_PIN       14

void setup () {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(115200);

  initiateRtc();

  DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();

  if (flag & DS3231AlarmFlag_Alarm1) {
    Serial.println("alarm one triggered");
    playMelody(BUZZER_PIN);
  }
  if (flag & DS3231AlarmFlag_Alarm2) {
    Serial.println("alarm two triggered");
    playMelody(BUZZER_PIN);
  }

  startServer();

  Serial.println("Going to sleep...");
  ESP.deepSleep(SLEEP_TIME);
}

float readVoltage () {
  int sensorValue = analogRead(A0);
  float voltage = (float)map(sensorValue, 13, 796, 0, 840) / 100;

  return voltage;
}

void loop () {  
  // do nothing here
}
