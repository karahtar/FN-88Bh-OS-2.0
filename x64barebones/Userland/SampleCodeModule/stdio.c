#include <stdint.h>
#include <stdarg.h>
#include "include/shell.h"
#include "include/stdio.h"
#include "include/string.h"
#include "include/stdlib.h"
#include "include/syscalls.h"

#define DEC 10
#define HEX 16

//prints in variable format
void printf(const char * fmt, ...) {
	va_list args;
	int index = 0;
	int aux = 0;
	char buffer[MAX_BUFFER];
	char c;
	va_start(args, fmt);

	while ( fmt[index] != '\0' ){
		c = fmt[index++];
		if ( c != '%'){
			putChar(c);
		}else{
			c = fmt[index++];
			switch(c){
				case 's':
					puts(va_arg(args, char*));
					break;
				case 'd':
					aux = itostr(buffer, va_arg(args, int), DEC);
					buffer[aux] = 0;
					puts(buffer);
					break;
				case 'x':
					aux = itostr(buffer, va_arg(args, int), HEX);
					buffer[aux] = 0;
					puts(buffer);
					break;
				default:
					break;
			}
		}
	}
	va_end(args);
}

void printTime(){
	time(STDOUT);
}

//prints 1 character
void putChar(char c) {
	write(STDOUT, &c, 1);
}

//prints a string
void puts(char * str) {
	write(STDOUT, str, strSize(str));
}

//gets 1 character
int getChar() {
	char c;
	//int num = 0;
	do {
		/*num = */read(STDIN, &c, 1);
	} while(/*num*/ c == 0);
	return c;
}

//gets input as string
int scan(char* str) {
	int length = 0;
	char c;
	char* aux =str;
	int goAway = 1;
	while(goAway) {
		c = getChar();
			if(c != 0 && c!= '\n') {
				if(c == '\b') {					
					if(aux<str){
						str--;
						*str = 0;
						length--;
					}
					
					}else{
						*str = c;
						str++;
						length++;
					}
				} else 
				{
					*str = 0;
					goAway = 0;
				}
			}
	return length;

}

//gets input in variable format
int scanf( const char * fmt, ...){
	va_list args;
	int ans = 0;
	int index = 0;
	int i;
	int n = 0;
	char num[MAX_BUFFER] = {0};
	char f;
	va_start(args, fmt);
	
	while (fmt[index] != 0){
		f = fmt[index++];
		if (f != '%'){
		printf("Formato invalido");
		}else{
			ans++;
			f = fmt[index++];
			switch(f){
				case 's':
					i = 0;
					char * v = va_arg(args, char *);
					do{
						n = getChar();
						if(n != '\n'){
							v[i++] = (char)n;
						}else{
							v[i++] = 0;
						}
					//	printf(v);
					}while(n != '\n');
				//	printf(v);
					break;
				case 'd':
					i = 0;
					do{
						n = getChar();
						if(n != '\n'){
							num[i++] = n;
						}else{
							num[i++] = 0;
						}
					}while(n != '\n');
					int * numb = va_arg(args, int *);
					numb[0] = strtoi(num);
					break;
				default:
					break;
			}
		}
	}
	va_end(args);
	return ans;
}


