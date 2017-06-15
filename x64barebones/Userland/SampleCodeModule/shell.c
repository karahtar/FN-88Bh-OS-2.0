#include <stdint.h>
#include "include/shell.h"
#include "include/stdio.h"
#include "include/string.h"
#include "include/syscalls.h"

#define MAX_MSG_SIZE 1024

static char buffer[MAX_BUFFER];
static char chatBuffer[MAX_BUFFER];
static int connectedUsers[255];
static int myId;
static int exit = -1;


void shell() {
	showMenu();
	do {
		printf("$> ");
		readInput();
	}while(exit == -1);
	printf("Ahora puede cerrar la consola\n");
	//return;
}

void readInput() {
	scanf("%s", buffer);

	if(strcmp(buffer, "chat") == 0){
		clearScreen();
		openChat();
	}else if(strcmp(buffer, "clear") == 0){
		clearScreen();
	}else if(buffer[0] == 'e' && buffer[1] == 'c' && buffer[2] == 'h' && buffer[3] == 'o' && buffer[4] == ' ') {
		printf("%s\n", buffer+5);
	}else if(strcmp(buffer, "exit") == 0) {
		exit = 1;
	}else if(strcmp(buffer, "help") == 0){
		showMenu();
	}else if(strcmp(buffer, "time") == 0){
		printTime();
		printf("\n");
	}else{
      printf("No se encontro la orden. Pruebe 'help' para mas informacion\n");
    }
    //printf("%s\n", buffer);
}

void openChat() {

	printf("Ahora puede enviar o recibir mensajes. Puede escribir:\n");
	chatMenu();
	getChatInput();

	return;
}

void chatMenu() {
	printf("enviar b [mensaje]          Para enviar un mensaje broadcast\n");
	printf("enviar p [mensaje]          Para enviar un mensaje personal\n");
	printf("recibir                     Para recibir mensajes si le corresponde\n");
	printf("id                          Imprime tu id\n");
	printf("clear                       Limpia la pantalla\n");
	printf("help                        Muestra el menu de ayuda\n");
	printf("users                       Muestra una lista de los usuarios conectados\n");
	printf("exit                        Para volver a la consola principal\n");
}

void getChatInput() {
	char bufAux[MAX_BUFFER];
	int active = 1;
	printf("llega?\n");
	sys_clear_msgs();
	printf("llega1\n");
	sys_connect();
	printf("llega2\n");
	myId = sys_get_network_id();
	printf("llega3\n");
	
	while(active) {
		printf("llega4\n");
		printf("$> ");
		scanf("%s", chatBuffer);
		active = readChatInput(bufAux);
	}

	sys_disconnect();
	clearScreen();
	shell();
}

int readChatInput(char* bufAux){
	if(chatBuffer[0] == 'e' && chatBuffer[1] == 'n' && chatBuffer[2] == 'v' && chatBuffer[3] == 'i' && chatBuffer[4] == 'a' && chatBuffer[5] == 'r' && chatBuffer[6] == ' ') {
		if(chatBuffer[7] == 'b' && chatBuffer[8] == ' ') {
			//sends broadcast message
			sys_broadcast(chatBuffer+9);
		//	printf("%s\n", buffer+9);
			printf("Mensaje broadcast enviado\n");
		}else if(chatBuffer[7] == 'p' && chatBuffer[8] == ' ') {
			printf("Ingrese el id del destinatario (0-255)\n");
			printf("$> ");
			int id;
			scanf("%d", id);
			if(id > 0xFF || id < 0) {
				printf("Id invalido\n");
			}else if(id == myId) {
				printf("No puede enviarse una mensaje a usted mismo\n");
			}else{
				int i;
				int active = 0; 
				int count = sys_get_active_users(connectedUsers);
				
				for(i = 0; i < count ; i++){
					if(id == connectedUsers[i]){
						//User is connected
						active = 1;
					}
				}
				if(active){
					//sends private message if id is connected
					sys_send(chatBuffer+9, id);
					printf("Mensaje enviado al usuario %d\n", id);
				}else{
					printf("El usuario %d no esta conectado\n", id);
				}
			}
		}else{
			printf("No se encontro la orden. Pruebe 'help' para mas informacion\n");
		}	
	}else if(strcmp(chatBuffer, "recibir") == 0) {
		//receives messages if there are any for me
		msg_desc msg_info;
		int b;
		if((b = sys_get_msg(bufAux, &msg_info, MAX_MSG_SIZE)) == -1) {
			printf("No hay mensajes nuevos\n");
		}else{
			do{
				printf("Mensajes nuevos:\n");
				printf("[Mensaje %s del usuario %d]  %s\n", msg_info.is_broadcast ? "publico" : "privado", msg_info.user, bufAux);
			}while((b = sys_get_msg(bufAux, &msg_info, MAX_MSG_SIZE)) != -1);
		}
	}else if(strcmp(chatBuffer, "id") == 0){
		printf("%d\n", myId);
	}else if(strcmp(chatBuffer, "clear") == 0){
		clearScreen();
	}else if(strcmp(chatBuffer, "help") == 0){
		chatMenu();
	}else if(strcmp(chatBuffer, "users") == 0){
		int i;
		int count = sys_get_active_users(connectedUsers);
		if(count > 0){
			printf("\nUsuarios conectados: \n");
		}else{
			printf("\nNo hay usuarios conectados\n");
		}
		for(i = 0; i < count; i++){
			printf("Usuario %d\n", connectedUsers[i]);
		}

	}else if(strcmp(chatBuffer, "exit") == 0){
		return 0;
	}else{
      printf("No se encontro la orden. Pruebe 'help' para mas informacion\n");
    }
    return 1;
}

void showMenu() {
	printf("Se pueden utilizar los siguientes comandos:\n");
	printf("chat                Abre el chat\n");
	printf("clear               Limpia la pantalla\n");
	printf("echo [mensaje]      Imprime [mensaje] en pantalla\n");
	printf("exit                Cierra la consola\n");
	printf("help                Muestra el menu de ayuda\n");
	printf("time                Imprime la hora\n");
}