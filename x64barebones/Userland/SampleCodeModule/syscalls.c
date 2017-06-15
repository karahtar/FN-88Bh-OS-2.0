#include <stdint.h>
#include <stdarg.h>
#include "include/shell.h"
#include "include/stdio.h"
#include "include/string.h"
#include "include/syscalls.h"


uint64_t read(int fileDescriptor, char* str, int strSize) {
	return _sysCall(READ, fileDescriptor, strSize, (uint64_t) str);
}

void write(int fileDescriptor, char* str, int strSize) {
	_sysCall(WRITE, fileDescriptor, strSize, (uint64_t) str);
}

void time(int fileDescriptor) {
	_sysCall(TIME, fileDescriptor, 0, 0);
}

void clearScreen(){
	_sysCall(CLEAR, STDOUT, 0, 0);
}

//sends a message to the network
uint64_t sys_send(char * msg, char user){
	return _sysCall(SYS_SEND_MSG, (uint64_t)msg, user, 0);
}

uint64_t sys_broadcast(char * msg){
	return _sysCall(SYS_SEND_BROADCAST, (uint64_t)msg, 0, 0);
}

//gets last unread message from queue
//returns -1 if empty, 0 if there is something
//it is copied in msg_info
uint64_t sys_get_msg(char * buf, msg_desc* msg_info, int max_size){
	return _sysCall(SYS_GET_MSG, (uint64_t)buf, (uint64_t)msg_info, max_size);
}

//resets the message queue
uint64_t sys_clear_msgs(){
	return _sysCall(SYS_CLEAR_MSGS, 0, 0, 0);
}

//connects a user to the network (to show it is connected)
uint64_t sys_connect(){
	return _sysCall(SYS_CONNECT, 0, 0, 0);
}

//disconnects a user to the network (to show it is disconnected)
uint64_t sys_disconnect(){
	return _sysCall(SYS_DISCONNECT, 0, 0, 0);
}

//fills vec with the active users (max 255). 
//returns the number written
uint64_t sys_get_active_users(int * vec){
	return _sysCall(SYS_GET_ACTIVE_USERS, (uint64_t)vec, 0, 0);
}

uint64_t sys_getAmountOfTicks(){
	return _sysCall(SYS_GETTICKS, 0, 0, 0);
}

uint64_t sys_get_rtc_data(int i){
	return _sysCall(SYS_GETRTCDATA, i, 0, 0);
}


uint64_t sys_get_network_id(){
	return _sysCall(SYS_GET_NETWORK_ID,0,0,0);
}