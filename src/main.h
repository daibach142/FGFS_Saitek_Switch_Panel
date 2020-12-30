/*****************************************************************************
** File     : main.h
** Project  : Flight Gear Flight Simulator to Saitek Switch Panel Interface
** Author   : Dave Attwood dave@attwoods.org.uk
** Copyright: (c) David Attwood 2020
** License  : GPL V3 or later
******************************************************************************
** */

#ifndef MAIN_H_
#define MAIN_H_

/* #define DEBUG */
char SwitchDevice[20];
char Simulator[20];

#define PORT_OUT	60000

unsigned int SwitchInput;	/* Compressed 3 bytes input from Switch Panel */

#endif /* MAIN_H_ */
