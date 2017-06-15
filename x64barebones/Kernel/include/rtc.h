#ifndef __RTC_H
#define __RTC_H
#include <stdint.h>


void rtcInit(void);
uint64_t getSeconds(void);
uint64_t getMinutes(void);
uint64_t getHours(void);
uint64_t getWeekDay(void);
uint64_t getDayOfMonth(void);
uint64_t getMonth(void);
uint64_t getYear(void);

#endif