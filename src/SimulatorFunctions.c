/*****************************************************************************
** File     : SimulatorFunctions.c
** Project  : Flight Gear Flight Simulator to Saitek Switch Panel Interface
** Author   : Dave Attwood dave@attwoods.org.uk
** Copyright: (c) David Attwood 2020
** License  : GPL V3 or later
******************************************************************************
** */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "main.h"


/*******************************************************************************************
 * Write a modification request to FGFS - code consists of a switch name
 * and an new value ("0" .. "4")
 * Data format expected by FGFS is defined in 'Protocol/stkswitch_input.xml' and is processed
 * with Nasal code in 'Nasal/Switch.nas'
 * ******************************************************************************************
 */
void SimulatorWrite(const char * property, char newValue) {

	struct sockaddr_in si_other;
	int s;
	char outputBuffer[100];

	int slen = sizeof(si_other);
	/* connect */
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("Socket create: ");
		exit(6);
	}

	memset((char*) &si_other, 0, sizeof(si_other));

	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT_OUT);

	if (inet_aton(Simulator, &si_other.sin_addr) == 0) {
		perror("inet_aton() failed: ");
		exit(7);
	}

	memset(outputBuffer, 0, 100);

	strcpy(outputBuffer, property);
	strcat(outputBuffer, ",");
	outputBuffer[strlen(outputBuffer)] = newValue;		/* append char as full of nulls */
	strcat(outputBuffer, "\n");
#ifdef DEBUG
	printf("SimulatorWrite: %s",outputBuffer);
#endif

	if (sendto(s, outputBuffer, strlen(outputBuffer), 0,
			(struct sockaddr*) &si_other, slen) == -1) {
		perror("sendto error: ");
		exit(8);
	}

	/* Disconnect socket */
	close(s);
}

