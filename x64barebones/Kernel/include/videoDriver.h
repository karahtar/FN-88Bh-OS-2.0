#ifndef _VIDEODRIVER_H_
#define _VIDEODRIVER_H_

#include <stdint.h>

void printNewLine(void);
void printOnScreen(char * str);
char * castIntToStr(int integer);
void printTime(void);
int _sysTime(int time);
void printNumber(int number);
void clearScreen(void);
void backspace(void);

#endif