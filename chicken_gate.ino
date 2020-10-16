#include "config.h"
#include "melody.h"
#include "rtc.h"
#include "server.h"
#include "voltage.h"
#include "motor.h"
#include "memory.h"

#define LED_PIN          2
#define BUTTON_PIN       14

void setup () {
  pinMode(LED_PIN,    OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);

  initiateMemory();

  // NOTE: enabling serial will occupy TX (GPIO 1) and RX (GPIO 3) ports that can't be used then
  // Serial.begin(115200);

  // NOTE - RTC will occupy pin 0 once initiated, we must count with that
  initiateRtc();

  // Deal with the real time clock module alarms first
  DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();

  if (flag & DS3231AlarmFlag_Alarm1) {
    openGate();
    sleep();
  }

  if (flag & DS3231AlarmFlag_Alarm2) {
    closeGate();
    sleep();
  }

  // Check if button is long pressed. If so, start the captive portal
  if (digitalRead(BUTTON_PIN) == HIGH) {
    delay(1500);
    if (digitalRead(BUTTON_PIN) == HIGH) {
      playMelody(melodyBeep, melodyBeepLength);
      startServer();
      sleep();
    }
  }

  // If button is not long pressed, open or close the gate based on its state
  unsigned long gateStatus = readFromMemory(STATE_ADDRESS);

  if (gateStatus == CLOSE) {
    openGate();
    sleep();
  }

  if (gateStatus == OPEN) {
    closeGate();
    sleep();
  }

  // We should never get here, but if something unexpected happens, go to sleep to save power
  sleep();
}

void sleep () {
  digitalWrite(LED_PIN, HIGH);
  ESP.deepSleep(0);
}

void loop () {
  // do nothing here
}
