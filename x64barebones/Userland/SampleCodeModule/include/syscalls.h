#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#define READ					0
#define WRITE					1
#define TIME					2
#define CLEAR   				3
#define SYS_SEND_MSG			4
#define SYS_SEND_BROADCAST		5
#define SYS_GET_MSG				6
#define SYS_CLEAR_MSGS			7
#define SYS_CONNECT 			8
#define SYS_DISCONNECT 			9
#define SYS_GET_ACTIVE_USERS 	10
#define SYS_GETTICKS 			11
#define SYS_GETRTCDATA 			12
#define SYS_GET_NETWORK_ID 		13

typedef struct{
	int is_broadcast;
	int user;
	struct{
		char day;
		char month;
		char year;
		char hour;
		char min;
	} time;
} msg_desc;

uint64_t _sysCall(uint64_t systemCall, uint64_t fileDescriptor, uint64_t str, uint64_t strSize);
void write(int fileDescriptor, char * str, int strSize);
uint64_t read(int fileDescriptor, char * str, int strSize);
void time(int fileDescriptor);
void clearScreen(void);
uint64_t sys_send(char * msg, char user);
uint64_t sys_broadcast(char * msg);
uint64_t sys_get_msg(char * buf, msg_desc* msg_info, int max_size);
uint64_t sys_clear_msgs(void);
uint64_t sys_connect(void);
uint64_t sys_disconnect(void);
uint64_t sys_get_active_users(int * vec);
uint64_t sys_getAmountOfTicks(void);
uint64_t sys_get_rtc_data(int i);
uint64_t sys_get_network_id(void);

#endif