#include "config.h"
#include "melody.h"
#include "rtc.h"
#include "server.h"
#include "voltage.h"
#include "motor.h"

// Define pins
#define LED_PIN          2
#define BUTTON_PIN       14

void setup () {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);

  // Serial.begin(115200);

  // NOTE - RTC will occupy pin 0 once initiated, we must count with that
  initiateRtc();

  DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();

  if (flag & DS3231AlarmFlag_Alarm1) {
    Serial.println("alarm one triggered");
    playMelody(melodyBeep, melodyBeepLength);
    openGate();
  }

  if (flag & DS3231AlarmFlag_Alarm2) {
    Serial.println("alarm two triggered");
    playMelody(melodyBeep, melodyBeepLength);
    playMelody(melodyBeep, melodyBeepLength);
    closeGate();
  }

  delay(500);

  if (digitalRead(BUTTON_PIN) == HIGH) {
    delay(1500);
    if (digitalRead(BUTTON_PIN) == HIGH) {
      playMelody(melodyUp, melodyUpLength);
      startServer();
    }
  }

  Serial.println("Going to sleep...");
  digitalWrite(LED_PIN, HIGH);
  ESP.deepSleep(0);
}

void loop () {
  // do nothing here
}
