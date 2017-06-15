#ifndef _STDIO_H_
#define _STDIO_H_

#define STDIN 0
#define STDOUT 1

void printf(const char * fmt, ...);
void printTime(void);
void putChar(char c);
void puts(char* str);
int getChar(void);
int scan(char* str);
int scanf(const char *fmt, ...);

#endif