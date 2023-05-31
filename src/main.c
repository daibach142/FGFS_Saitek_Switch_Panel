/*****************************************************************************
 ** File     : main.c
 ** Project  : Flight Gear Flight Simulator to Saitek Switch Panel Interface
 ** Author   : Dave Attwood dave@attwoods.org.uk
 ** Copyright: (c) David Attwood 2020
 ** License  : GPL V3 or later
 ******************************************************************************
 ** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>


#include "config.h"
#include "SimulatorPanel.h"

#include "SwitchFunctions.h"
#include "SimulatorFunctions.h"

// Issue #1: compilation fails
char SwitchDevice[20];
char Simulator[20];

unsigned int SwitchInput;	/* Compressed 3 bytes input from Switch Panel */
// Issue # 1: end


static void processArguments(int argc, char *argv[]);
static void readInputs(void);
static void processSwitchInput(int switches);
static void SimulatorInit(int read);

/*****************************************************************************
 *
 * The program flow is as follows:
 *  	Read the SWITCH Panel blocking
 *  	On switch setting changed, reflect change to the Simulator
 *  repeat
 *  *************************************************************************/

int main(int argc, char *argv[]) {

	processArguments(argc, argv);

	printf(
			"This is " PACKAGE_NAME " Version " PACKAGE_VERSION " to FGFS at %s\n", Simulator);

	int firstRead = SwitchInit();
	SimulatorInit(firstRead);

	while (1) {
		readInputs();
	}
	return EXIT_SUCCESS;
}

/******************************************************************************
 * processArguments
 * Expected:
 *    -s or --switch	: device address for Saitek Switch Panel
 *    -f or --flightgear : IP address of FGFS server
 *
 * If linked with libudev, device will be discovered and need not be specified
 * Otherwise, both arguments are required
 * *****************************************************************************/

void processArguments(int argc, char *argv[]) {

	while (1) {
		int c;
		int optionIndex = 0;
		static struct option longOptions[] = {
				{ "switch", required_argument, 0, 's' },
				{ "flightgear",	required_argument, 0, 'f' },
				{ 0, 0, 0, 0 }
		};
		c = getopt_long(argc, argv, "r:s:f:", longOptions, &optionIndex);
		if (c == -1)
			break;

		switch (c) {

		case 's':
			strncpy(SwitchDevice, optarg, sizeof(SwitchDevice));
			break;

		case 'f':
			strncpy(Simulator, optarg, sizeof(Simulator));
			break;

		default:
			break; /* not good argument */
		}
	}
}

/**************************************************************************
 * Read inputs from the Panel and (with retracting gear) from Simulator
 * Update the Panel view directly if selection of instruments on display has
 * changed.
 */
void readInputs(void) {
	unsigned int result = SwitchRead();	/* blocking */
	if (result != SwitchInput) {
#ifdef DEBUG
		printf("Read Inputs STKRP: %06x\n", result);
#endif
		processSwitchInput(result);
		SwitchInput = result;
	}
#ifdef GEARRETRACTS
	SimulatorRead();
#endif
}

/**************************************************************************
 * Process all the switch changes since previous read
 * Issue a 'write' to FGFS giving the new state for each switch
 */
void processSwitchInput(int switches) {
	int changed = (switches ^ SwitchInput) & SWITCHMASK;
	if (changed) {	/* not for magnetos or landing gear */
		for (int i = 0; i < sizeof(switchDef) / sizeof(FGIF); i++) {
			if (switchDef[i].locator & changed) {  /* this ones changed */
				switchValues[i] = (~switchValues[i]) & switchDef[i].locator; /* only relevant bit */
				SimulatorWrite(switchDef[i].switchTarget,
						(switchValues[i]) ? '1' : '0');
			}
		}
	}
	int magSetting = switches & MAGMASK;
	if (magSetting != magnetoValue) { /* magneto switch changed */
		for (int i = 0; i < 5; i++) {
			if (magSetting & magnetoMap[i]) {
				magnetoValue = magSetting;
				SimulatorWrite(magnetoSim, (char)(i + '0'));
				if (i == 4) SimulatorWrite(magStarter, '1');
				else SimulatorWrite(magStarter, '0');
				break;
			}
		}
	}
	/* alternative use of gear up/down to prime out/in */
#ifdef GEARRETARGET
	if ((switches ^ SwitchInput) & GEARMASK) {
		SimulatorWrite(geartarget, (switches & GEARUP) ? '1' : '0');
	}
#endif
}

/**************************************************************************
 * SimulatorInit
 *    sets up initial switch settings on simulator
 *
 */
void SimulatorInit(int readValue) {
	for (int i = 0; i < 13; i++) {
		switchValues[i] = readValue & switchDef[i].locator;
	}
	SwitchInput = readValue;
	magnetoValue = readValue & MAGMASK;

/* first time - make simulator settings match switch panel */

	/* switches generally are boolean, although these values work for double, too */
	for (int i = 0; i < sizeof(switchDef) / sizeof(FGIF); i++) {
		SimulatorWrite(switchDef[i].switchTarget, (switchValues[i]) ? '1' : '0');
		usleep(50000);
	}
	/* make magneto value match (0..4) */
	for (int i = 0; i < 5; i++) {
		if (magnetoValue & magnetoMap[i]) {
			SimulatorWrite(magnetoSim, (char)(i + '0'));
			break;
		}
	}

}

