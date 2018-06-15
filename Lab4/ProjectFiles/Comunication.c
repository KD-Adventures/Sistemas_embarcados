#include "Comunication.h"

void sendString(char* string) {
	int i = 0;
	int length = strlen(string);
	
	for(i = 0; i < length; i++)
	{
		sendUART(string[i]);
	}
}