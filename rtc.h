#ifndef RTC_H
#define RTC_H

#include <RtcDS3231.h>
#include <Wire.h>

static RtcDS3231<TwoWire> Rtc(Wire);

void initiateRtc                  ();

void setCompileDateIfNotValid     ();

void clearAlarms                  ();

void printCurrentTime             ();

#endif //RTC_H