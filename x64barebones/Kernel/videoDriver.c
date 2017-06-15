#include <videoDriver.h>

#define ROWS 25
#define COLS 80
#define GMT 3

char* vidStart = (char *) 0xB8000;

static int lastUsed = 0;

void printNewLine(void) {
	lastUsed = ((lastUsed / (COLS * 2)) + 1) * COLS * 2;
}

void printOnScreen(char * str) {
	int i = 0;
	while(str[i] != 0) {
		if(str[i] == '\n') {
			printNewLine();
			i++;
		} else {
		*(vidStart+lastUsed) = str[i];
		*(vidStart+lastUsed+1) = 0x0F;
		lastUsed += 2;
		i++;
		}
		if(lastUsed > ROWS*COLS*2) {
			char* aux = 0;
			aux[0] = str[i-1];		//saves the first character of the screen
			aux[1] = '\0';
			clearScreen();			//clears the screen, including the first character
			printOnScreen(aux);		//prints the first character on the screen
		}
	}
	return;
}

static char ans[5] = {0};

char * castIntToStr(int integer) {
	char aux[5] = {0};
	int i=0, j;
	do
	{
		aux[i] = integer % 10 + '0';
		integer = integer/10;
		i++;
	} while (i < 2);

	for(j=0; j<i; j++) {
		ans[j] = aux[i-j-1];
	}
	return ans;
}

//IMPRIME BIEN HASTA TIPO 8PM, ARREGLAR
void printTime(void) {

	int seconds = 0;
	int secondsBCD;
	int minutes = 0;
	int minutesBCD;
	int hours = 0;
	int hoursBCD;

	hoursBCD = (int)_sysTime(4);		
	hours += ((hoursBCD & 0xF0)>>4)*10;
	hours += (hoursBCD & 0x0F);
//	printOnScreen("Hora ");
	printNumber(hours-GMT);
	minutesBCD = (int)_sysTime(2);
	minutes += ((minutesBCD & 0xF0)>>4)*10;
	minutes += (minutesBCD & 0x0F);
	printOnScreen(":");
	printNumber(minutes);
	secondsBCD = (int)_sysTime(0);
	seconds += ((secondsBCD & 0xF0)>>4)*10;
	seconds += (secondsBCD & 0x0F);
	printOnScreen(":");
	printNumber(seconds);
}

void printNumber(int number) {
	// cast number from int to char
	char * strToPrint = castIntToStr(number);
	printOnScreen(strToPrint);
	return;
}

void clearScreen(void) {
	lastUsed = 0;
	while(lastUsed != ROWS*COLS*2) {
		printOnScreen(" ");
	}
	lastUsed = 0;
}

void backspace(void) {
	lastUsed -= 2;
	printOnScreen(" ");
	lastUsed -= 2;
}