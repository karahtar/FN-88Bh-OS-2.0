#include <keyboardDriver.h>
#include <videoDriver.h>

static char buffer[MAX_BUFFER] = {0};
//static int shiftPressed = 0;
static int mayusPressed = -1;
static int endOfLine = -1;
static unsigned int read_index = 0;
static unsigned int write_index = 0;


static char KEYS_MAPPING[] = {0, 'ESC', '1', '2', '3', '4', '5', '6', '7', '8', '9', 			// 0 - 10
									'0', '\'', '¿', 0, '    ', 'q', 'w', 'e', 'r', 't',// 11 - 20
								  	'y', 'u', 'i', 'o', 'p', '´', '+', '\n', 0, 'a',			// 21 - 30
									's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0, '{', 			// 31 - 40
								  	'|', 'SHIFT', '}', 'z', 'x', 'c', 'v', 'b', 'n', 'm',  		// 41 - 50
								  	',', '.', '-', 0, '*', 0, ' ', 0, 0, 0};				// 51 - 60										// 55 - 62

static char SHIFT_KEYS_MAPPING[] = {0, 'ESC', '!', '\0', '#', '$', '%', '&', '/', '(', ')', 	// 0 - 10
									'=', '?', '¡', 0, '    ', 'Q', 'W', 'E', 'R', 'T',// 11 - 20
								  	'Y', 'U', 'I', 'O', 'P', '¨', '*', '\n', 0, 'A',			// 21 - 30
									'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0, '[', 			// 31 - 40
								  	'°', 'SHIFT', ']', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',  		// 41 - 50
								  	';', ':', '_', 0, '*', 0, ' ', 0, 0, 0};				// 51 - 60



//NO LEE EL SHIFT, SOLO CAPSLOCK
//EL BACKSPACE BORRA BIEN SALVO QUE SEA LA ULTIMA LETRA DE UNA PALABRA (PONER 0!)
//SE GUARDAN ESPACIOS RANDOM EN BUFFER CUANDO ESTA EN MAYUSCULA
//returns the key corresponding to scanCode from the key map
void chooseKey(int scanCode) {
	char printc[2] = {0};
	if(scanCode >= 0 &&  scanCode < MAX_KEYPRESSED) {					//key pressed
	//	if(scanCode == 42 || scanCode == 54) {		//shift
	//		shiftPressed = 1;
	//	} else 
		if(scanCode == 14) {					//backspace
			backspace();
//			if(write_index != 0){
				buffer[write_index] = 0;
				write_index--;
				write_index--;
		} else if(scanCode == 28) {					//enter
			printNewLine();
			endOfLine = 1;
		} else if(scanCode == 58) {					//capsLock
			mayusPressed *= -1;
		} else {
			/*if(shiftPressed) {
				if((mayusPressed == -1)  || (scanCode >= 2 && scanCode <= 13)){
					printc[0] = SHIFT_KEYS_MAPPING[scanCode];
					printOnScreen(printc);
				}
				else{
					printc[0] = KEYS_MAPPING[scanCode];
					printOnScreen(printc);			
				}	
			}else */if((mayusPressed == 1) && !(scanCode >= 2 && scanCode <= 13)){
						printc[0] = SHIFT_KEYS_MAPPING[scanCode];
						printOnScreen(printc);
					}else{
						printc[0] = KEYS_MAPPING[scanCode];
						printOnScreen(printc);
					}
		}
	}
//	else if(scanCode == 170 || scanCode == 182){
//			shiftPressed = 0;
//	}
}


//adds a key from the key map to the buffer
void addKeyToBuffer() {
	char scanCode = _readKey();
	chooseKey(scanCode);

/*	if(shiftPressed == 1){
		if(mayusPressed == -1  || (scanCode >= 2 && scanCode<=13)){
			buffer[(write_index++) % (MAX_BUFFER-1)] = SHIFT_KEYS_MAPPING[(int)scanCode];
		}else{
			buffer[(write_index++) % (MAX_BUFFER-1)] = KEYS_MAPPING[(int)scanCode];
		}
	}else */if(mayusPressed == 1 && !(scanCode >= 2 && scanCode<=13)){
		buffer[(write_index++) % (MAX_BUFFER-1)] = SHIFT_KEYS_MAPPING[(int)scanCode];
	}else{
		buffer[(write_index++) % (MAX_BUFFER-1)] = KEYS_MAPPING[(int)scanCode];
	}
}

int readFromBuffer(int size, char * str) {
	if(read_index == write_index || write_index-read_index < size){
		return 0;
	}
	int s = size;
	int i = 0;
	while(size > 0 && endOfLine != -1){
		if(buffer[read_index % (MAX_BUFFER-1)] == '\n'){
			endOfLine = -1;
		}
		 str[i++] = buffer[(read_index++) % (MAX_BUFFER-1)];
		size--;
	}
	return s;
}