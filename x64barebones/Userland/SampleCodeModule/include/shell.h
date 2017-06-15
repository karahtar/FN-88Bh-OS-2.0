#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdint.h>

#define MAX_BUFFER 1000

void shell(void);
void readInput(void);
void openChat(void);
void chatMenu(void);
void getChatInput(void);
int readChatInput(char* buf);
void showMenu(void);

#endif