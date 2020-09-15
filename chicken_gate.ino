#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Servo.h>
#include <Wire.h>
#include <RtcDS3231.h>

#include "config.h"
#include "melody.h"

#define LED_PIN          2
#define SLEEP_TIME       3*60*60e6 // the highest sleep period that works reliably
#define BUZZER_PIN       14

RtcDS3231<TwoWire> Rtc(Wire);

void setup () {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(115200);
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!Rtc.IsDateTimeValid()) 
  {
      if (Rtc.LastError() != 0)
      {
          // we have a communications error
          // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
          // what the number means
          Serial.print("RTC communications error = ");
          Serial.println(Rtc.LastError());
      }
      else
      {
          Serial.println("RTC lost confidence in the DateTime!");
          Rtc.SetDateTime(compiled);
      }
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) 
  {
      Serial.println("RTC is older than compile time!  (Updating DateTime)");
      Rtc.SetDateTime(compiled);
  }

  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);

  // throw away any old alarm state
  Rtc.LatchAlarmsTriggeredFlags();

  RtcDateTime alarmTime = now + 30; // seconds into the future
  DS3231AlarmOne alarm1(
          alarmTime.Day(),
          alarmTime.Hour(),
          alarmTime.Minute(), 
          alarmTime.Second(),
          DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(alarm1);

  DS3231AlarmTwo alarm2(
          alarmTime.Day(),
          alarmTime.Hour(),
          alarmTime.Minute() + 1, 
          DS3231AlarmTwoControl_HoursMinutesMatch);
  Rtc.SetAlarmTwo(alarm2);
  // ESP.deepSleep(SLEEP_TIME);
}

float readVoltage () {
  int sensorValue = analogRead(A0);
  float voltage = (float)map(sensorValue, 13, 796, 0, 840) / 100;

  return voltage;
}

void loop () {

  RtcDateTime now = Rtc.GetDateTime();
  Serial.print("Date:");
  Serial.print(now.Year(), DEC);
  Serial.print('/');
  Serial.print(now.Month(), DEC);
  Serial.print('/');
  Serial.print(now.Day(), DEC);
  Serial.print(" Time:");
  Serial.print(now.Hour(), DEC);
  Serial.print(':');
  Serial.print(now.Minute(), DEC);
  Serial.print(':');
  Serial.println(now.Second(), DEC);

  DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();

  if (flag & DS3231AlarmFlag_Alarm1) {
    Serial.println("alarm one triggered");
    playMelody(BUZZER_PIN);
  }
  if (flag & DS3231AlarmFlag_Alarm2) {
    Serial.println("alarm two triggered");
  }
  
  delay(1000); // one second
}
