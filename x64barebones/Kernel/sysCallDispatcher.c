#include <videoDriver.h>
#include <sysCallDispatcher.h>
#include <keyboardDriver.h>
#include <rtlDriver.h>
#include <rtc.h>


typedef uint64_t (* rtc_data)();

rtc_data functions[] = {
	getSeconds,
	getMinutes,
	getHours,
	getWeekDay,
	getDayOfMonth,
	getMonth,
	getYear
};


uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {

	int syscall = (int) rdi;
	int fileDescriptor = (int) rsi;
	//char * str = (char*) rdx;
	int strSize = (int) rcx;

	switch(syscall) {
		case READ: {
			sys_read(fileDescriptor, rdx, strSize);
			break;
		}
		case WRITE: {
			sys_write(fileDescriptor, rdx, strSize);
			break;
		}
		case TIME: {
			printTime();
			return 0;
			break;
		}
		case CLEAR: {
			clearScreen();
			return 0;
			break;
		}
		case SYS_SEND_MSG: {
			_send_msg(rsi, rdx, rcx);
			break;
		}
		case SYS_SEND_BROADCAST: {
			_send_broadcast(rsi, rdx, rcx);
			break;
		}
		case SYS_GET_MSG: {
			_get_msg(rsi, rdx, rcx);
			break;
		}	
		case SYS_CLEAR_MSGS: {
			_clear_msgs(rsi, rdx, rcx);
			break;
		}
		case SYS_CONNECT: {
			_connect(rsi, rdx, rcx);
			break;
		}
		case SYS_DISCONNECT: {
			_disconnect(rsi, rdx, rcx);
			break;
		}
		case SYS_GET_ACTIVE_USERS: {
			_get_active_users(rsi, rdx, rcx);
			break;
		}
		case SYS_GETTICKS: {
			_get_ticks(rsi, rdx, rcx);
			break;
		}
		case SYS_GETRTCDATA: {
			_get_rtc_data(rsi, rdx, rcx);
			break;
		}
		case SYS_GET_NETWORK_ID: {
			_get_network_id(rsi, rdx, rcx);
			break;
		}
	}
	return 0;
}

uint64_t sys_read(uint64_t fileDescriptor, uint64_t str, uint64_t strSize) {
	if(fileDescriptor == STDIN) {
		int ans;
		char* strAux = (char*) str;
		ans = readFromBuffer(strSize,strAux);
		return ans;
	}
	return 0;
}

uint64_t sys_write(uint64_t fileDescriptor, uint64_t str, uint64_t strSize) {
	char* strAux = (char*) str;
	if(fileDescriptor == STDOUT)
		printOnScreen(strAux);
	return 0;
}

uint64_t _get_msg(uint64_t buf, uint64_t msg_info, uint64_t max_size){
	return rtl_next_msg((char*)buf, (void*)msg_info ,max_size);
}


uint64_t _send_msg(uint64_t msg, uint64_t dst, uint64_t dummy){
	rtl_send((char*)msg, (uint8_t)dst);
	return 0;
}

uint64_t _send_broadcast(uint64_t msg, uint64_t dummy1, uint64_t dummy2){
	rtl_send((char*)msg, -1);
	return 0;
}


uint64_t _clear_msgs(uint64_t dummy1, uint64_t dummy2, uint64_t dummy3){
	rtl_clear_msgs();
	return 0;
}

uint64_t _connect(uint64_t dummy1, uint64_t dummy2, uint64_t dummy3){
	rtl_notify_connection(1);
	return 0;
}

uint64_t _disconnect(uint64_t dummy1, uint64_t dummy2, uint64_t dummy3){
	rtl_notify_connection(0);
	return 0;
}

uint64_t _get_active_users(uint64_t vec, uint64_t dummy1, uint64_t dummy2){
	return rtl_get_active_users((int*)vec);
}

uint64_t _get_ticks(uint64_t dummy, uint64_t dummy2, uint64_t dummy3){
	return getTicks();
}

uint64_t _get_network_id(uint64_t dummy, uint64_t dummy1, uint64_t dummy2){
	return rtl_get_id();
}


uint64_t _get_rtc_data(uint64_t which, uint64_t dummy1, uint64_t dummy2){
	return functions[which]();
}

