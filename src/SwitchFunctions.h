/*****************************************************************************
** File     : SwitchFunctions.h
** Project  : Flight Gear Flight Simulator to Saitek Switch Panel Interface
** Author   : Dave Attwood dave@attwoods.org.uk
** Copyright: (c) David Attwood 2020
** License  : GPL V3 or later
******************************************************************************
*/
#ifndef SWITCHFUNCTIONS_H
#define SWITCHFUNCTIONS_H

// Saitek Switch Panel 'udev' identifiers
// Vendor
#define SAITEK_VENDOR_ID	"06a3"
// Device
#define SAITEK_SWITCH_DEVICE_ID	"0d67"


// returned 3 bytes from Saitek Switch Panel
// bit positions of flags, per packed integer
// packed as:
// 1st byte 0x??0000
// 2nd byte 0x00??00
// 3rd byte 0x0000??

#define	BATTERY		0x010000
#define ALT			0x020000
#define	AVIONICS 	0x040000
#define FUELPUMP	0x080000
#define DEICE		0x100000
#define PITOTHEAT	0x200000
#define COWLCLOSE	0x400000
#define PANELLIGHT	0x800000
#define BEACON		0x000100
#define NAVLIGHTS	0x000200
#define STROBE		0x000400
#define TAXI		0x000800
#define LANDING		0x001000
#define SWITCHMASK	( BATTERY | ALT | AVIONICS | FUELPUMP | DEICE | PITOTHEAT | COWLCLOSE | \
					PANELLIGHT | BEACON | NAVLIGHTS | STROBE | TAXI | LANDING )

#define GEARUP		0x000004
#define GEARDOWN	0x000008
#define GEARMASK	( GEARUP | GEARDOWN )

#define MAGOFF		0x002000
#define MAGR		0x004000
#define MAGL		0x008000
#define MAGBOTH		0x000001
#define MAGSTART	0x000002
#define MAGMASK		( MAGOFF | MAGR | MAGL | MAGBOTH | MAGSTART )

// Write data for Gear LEDS
// LED may be yellow if RED and GREEN asserted
#define	NOSERED		0x08
#define NOSEGREEN	0x01
#define NOSEOFF		~(NOSERED | NOSEGREEN)
#define LEFTRED		0x10
#define LEFTGREEN	0x02
#define LEFTOFF		~(LEFTRED | LEFTGREEN)
#define RIGHTRED	0x20
#define RIGHTGREEN	0x04
#define RIGHTOFF	~(RIGHTRED | ROGHTGREEN)
#define ALLOFF      0x00
#define LEDFIELD	0x3f

typedef struct iface {
        int locator;
        char * switchTarget;
} FGIF  ;


/*********************************************************************************************************
 * View accessible functions
 * *******************************************************************************************************
 */

int SwitchInit(void);
unsigned int SwitchRead(void);
void SwitchWrite(unsigned char  x);

#endif /* SWITCHFUNCTIONS_H */
