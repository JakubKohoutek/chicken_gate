#include <Servo.h>

#include "config.h"
#include "melody.h"
#include "rtc.h"
#include "server.h"
#include "voltage.h"

// Define pins
#define LED_PIN          2
#define BUTTON_PIN       14

void setup () {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);

  initiateRtc();

  DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();

  if (flag & DS3231AlarmFlag_Alarm1) {
    Serial.println("alarm one triggered");
    playMelody(melodyBeep, melodyBeepLength);
  }

  if (flag & DS3231AlarmFlag_Alarm2) {
    Serial.println("alarm two triggered");
    playMelody(melodyBeep, melodyBeepLength);
    playMelody(melodyBeep, melodyBeepLength);
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
  playMelody(melodyDown, melodyDownLength);
  digitalWrite(LED_PIN, HIGH);
  ESP.deepSleep(0);
}

void loop () {
  // do nothing here
}
