#include "Comunication.h"

void sendString(char* string) {
	int i = 0;
	int length = strlen(string);
	
	for(i = 0; i < length; i++)
	{
		sendUART(string[i]);
	}
}

void sendFloat(double t) {
	char buffer[64];
	floatToString(t, buffer, 64, 10, 5, 5);
	sendString(buffer);
}