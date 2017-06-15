#include <stdint.h>
#include "include/shell.h"
#include "include/stdio.h"
#include "include/string.h"


int strcmp(char * str1, char * str2) {
//	int i = 0;
//	int different = 0;
//	printf("%s\n", str1);
//	printf("%s\n", str2);
	/*
	while(!different && str1[i] != 0 && str2[i] != 0) {
		if(str1[i] != str2[i])
			different = 1;
	}
	return !different;
	*/
	while(*str1 && *str2 && *str1 == *str2){
		str1++;
 		str2++;
	}
	return *str1 - *str2;
}

int strSize(char * str) {
	int i = 0;
	while(str[i] != '\0') {
		i++;
	}
	return i;
}

int strcpy(char * to, const char * from) {
	int i;
	for (i = 0; from[i] != '\0'; i++)
		to[i] = from[i];
	to[i] = '\0';
	return i;
}
