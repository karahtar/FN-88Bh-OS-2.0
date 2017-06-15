#ifndef _KEYBOARDDRIVER_H_
#define _KEYBOARDDRIVER_H_

#define MAX_KEYPRESSED 120
#define MAX_BUFFER 1000

#define TRUE 1
#define FALSE 0
#define ERROR -1

int _readKey(void);

void chooseKey(int scanCode);
void addKeyToBuffer(void);
int readFromBuffer(int size,char * str);

#endif