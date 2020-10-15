#include "config.h"
#include "melody.h"
#include "rtc.h"
#include "server.h"
#include "voltage.h"
#include "motor.h"
#include "memory.h"

// Define pins
#define LED_PIN          2
#define BUTTON_PIN       14

void setup () {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);

  initiateMemory();

  // NOTE: enabling serial will occupy TX (GPIO 1) and RX (GPIO 3) ports that can't be used then
  // Serial.begin(115200);

  // NOTE - RTC will occupy pin 0 once initiated, we must count with that
  initiateRtc();

  DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();

  if (flag & DS3231AlarmFlag_Alarm1) {
    openGate();
  }

  if (flag & DS3231AlarmFlag_Alarm2) {
    closeGate();
  }

  delay(500);

  if (digitalRead(BUTTON_PIN) == HIGH) {
    delay(1500);
    if (digitalRead(BUTTON_PIN) == HIGH) {
      playMelody(melodyBeep, melodyBeepLength);
      startServer();
    }
  } else if (readFromMemory(STATE_ADDRESS) == CLOSE) {
    openGate();
  } else if (readFromMemory(STATE_ADDRESS) == OPEN) {
    closeGate();
  }

  digitalWrite(LED_PIN, HIGH);
  ESP.deepSleep(0);
}

void loop () {
  // do nothing here
}
