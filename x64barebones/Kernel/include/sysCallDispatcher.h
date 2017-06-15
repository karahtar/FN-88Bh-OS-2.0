#ifndef _SYSCALLDISPATCHER_H_
#define _SYSCALLDISPATCHER_H_

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

#define STDIN  0
#define STDOUT 1
#define ETH	   2

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);
uint64_t sys_read(uint64_t fileDescriptor, uint64_t str, uint64_t strSize);
uint64_t sys_write(uint64_t fileDescriptor, uint64_t str, uint64_t strSize);
uint64_t _get_msg(uint64_t buf, uint64_t msg_info, uint64_t max_size);
uint64_t _send_msg(uint64_t msg, uint64_t dst, uint64_t dummy);
uint64_t _send_broadcast(uint64_t msg, uint64_t dummy1, uint64_t dummy2);
uint64_t _clear_msgs(uint64_t dummy1, uint64_t dummy2, uint64_t dummy3);
uint64_t _connect(uint64_t dummy1, uint64_t dummy2, uint64_t dummy3);
uint64_t _disconnect(uint64_t dummy1, uint64_t dummy2, uint64_t dummy3);
uint64_t _get_active_users(uint64_t vec, uint64_t dummy1, uint64_t dummy2);
uint64_t _get_ticks(uint64_t dummy, uint64_t dummy2, uint64_t dummy3);
uint64_t _get_network_id(uint64_t dummy, uint64_t dummy1, uint64_t dummy2);
uint64_t _get_rtc_data(uint64_t which, uint64_t dummy1, uint64_t dummy2);


#endif