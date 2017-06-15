#ifndef __RTLDRIVER_H
#define __RTLDRIVER_H

void rtlSend();
void rtlHandler();
void rtl_init();
int rtl_next_msg(char* buf, void * info, int max_size);
void rtl_notify_connection(int connect);
void rtl_clear_msgs();

int rtl_get_active_users(int * vec);
int rtl_get_id();
void rtl_send(char * msg, int dst);
void rtl_notify_connection(int connect);


void rtlPrintMac();
void printDetails();
uint8_t nextDesc(uint8_t current);
#endif