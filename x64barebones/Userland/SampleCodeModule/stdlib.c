#include <stdint.h>
#include "include/string.h"

//converts a string into an int
int strtoi(char * str) {
	int i;
	int n = 0;
	int stringSize = strSize(str);
	for(i = stringSize; i >= 0; i--) {
		n += (int)(str[i] - '0') * (stringSize - i);
	}
	return n;
}

//converts an int into a string
//from http://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
char * itostr(char* buff, int n, int radix) {
	char const digit[] = "0123456789";
    char* p = buff;
    if(n < 0){
        *p++ = '-';
        n *= -1;
    }
    int shifter = n;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/radix;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[n%radix];
        n = n/radix;
    }while(n);
    return buff;
}

//returns 1 if c is a digit 0-9
int isdigit(char c) {
	int n = (int) c;
	return n >= '0' && n <= '9'? 1 : 0;
}