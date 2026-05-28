#pragma once
#include <M5Unified.h>
#include <time.h>
#include <sys/time.h>

// Drop-in stand-ins for the M5StickCPlus RTC struct types so the rest of
// the firmware doesn't have to be rewritten. Time is kept in the ESP32
// system clock (settimeofday/localtime_r), which works on every board
// with or without hardware RTC. M5StickC Plus has a battery-backed RTC;
// M5StickS3 does not — on S3 the clock resets at power loss but the
// bridge re-syncs within seconds of pairing.
struct RTC_TimeTypeDef { uint8_t Hours, Minutes, Seconds; };
struct RTC_DateTypeDef { uint8_t WeekDay, Month, Date; uint16_t Year; };

inline void clockSetEpoch(time_t local) {
  struct timeval tv = { local, 0 };
  settimeofday(&tv, nullptr);
}

inline void clockGetTime(RTC_TimeTypeDef* tm) {
  time_t now; time(&now);
  struct tm lt; gmtime_r(&now, &lt);
  tm->Hours = lt.tm_hour; tm->Minutes = lt.tm_min; tm->Seconds = lt.tm_sec;
}

inline void clockGetDate(RTC_DateTypeDef* dt) {
  time_t now; time(&now);
  struct tm lt; gmtime_r(&now, &lt);
  dt->WeekDay = lt.tm_wday;
  dt->Month   = lt.tm_mon + 1;
  dt->Date    = lt.tm_mday;
  dt->Year    = lt.tm_year + 1900;
}
