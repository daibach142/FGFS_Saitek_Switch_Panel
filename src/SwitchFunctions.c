/*****************************************************************************
 ** File     : SwitchFunctions.c
 ** Project  : Flight Gear Flight Simulator to Saitek Switch Panel Interface
 ** Author   : Dave Attwood dave@attwoods.org.uk
 ** Copyright: (c) David Attwood 2020
 ** License  : GPL V3 or later
 ******************************************************************************
 */
#include "config.h"

#ifdef HAVE_LIBUDEV_H
#include <libudev.h>
#endif

#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "SwitchFunctions.h"
#include "main.h"

/**************************************************************************************************
 * Local variables
 * ************************************************************************************************
 */
static int SwitchFD;				/* For reading and writing Switch */
static unsigned char buffer[4];		/* Holds (last read) Switch input */


/** ************************************************************************
 *  Lets go find the device (/dev/hidraw<x>)
 *  using the UDEV library and enable the STK Switch Panel.
 *  If we don't have the UDEV library, use the value set as program argument
 *  ************************************************************************
 **/

int SwitchInit() {

#ifdef HAVE_LIBUDEV_H
	static const char * SwitchDevice;		/* override the main.h definition? */
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices;
	struct udev_list_entry *dev_list_entry;
	struct udev_device *dev;

	udev = udev_new();
	if (!udev) {
		printf("Can't create udev\n");
		exit(1);
	}

	/* create list of hidraw */
	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "hidraw");
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);
	udev_list_entry_foreach(dev_list_entry, devices)
	{
		const char *path;
		/* Get the filename of the /sys entry for the device
		 and create a udev_device object (dev) representing it */
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);

		/* usb_device_get_devnode() returns the path to the device node
		 itself in /dev. */
		SwitchDevice = udev_device_get_devnode(dev);  // eg. "/dev/hidraw4"

		/* The device pointed to by dev contains information about
		 the hidraw device. In order to get information about the
		 USB device, get the parent device with the
		 subsystem/devtype pair of "usb"/"usb_device". This will
		 be several levels up the tree, but the function will find
		 it.*/
		dev = udev_device_get_parent_with_subsystem_devtype(dev, "usb",
				"usb_device");
		if (!dev) {
			printf("Unable to find parent usb SwitchDevice.");
			exit(1);
		}

		/* From here, we can call get_sysattr_value() for each file
		 in the device's /sys entry. The strings passed into these
		 functions (idProduct, idVendor, serial, etc.) correspond
		 directly to the files in the directory which represents
		 the USB device. Note that USB strings are Unicode, UCS2
		 encoded, but the strings returned from
		 udev_device_get_sysattr_value() are UTF-8 encoded. */
		if (strcmp(udev_device_get_sysattr_value(dev, "idVendor"),
		SAITEK_VENDOR_ID) == 0
				&& strcmp(udev_device_get_sysattr_value(dev, "idProduct"),
				SAITEK_SWITCH_DEVICE_ID) == 0) {
			// Gotcha!!
			udev_device_unref(dev);
			break;
		} else
			SwitchDevice = NULL;
		udev_device_unref(dev);
	}
	/* Free the enumerator object */
	udev_enumerate_unref(enumerate);
	udev_unref(udev);
#endif
	if (SwitchDevice != NULL) {
		printf("Saitek Switch on %s\n", SwitchDevice);

		/* Note this is a blocking read, be careful when implementing GEAR movement monitoring */
		SwitchFD = open(SwitchDevice, O_RDWR);

		if (SwitchFD < 0) {
			perror("Unable to open SwitchDevice");
			exit(2);
		}
	} else {
		printf("Saitek Switch Panel not plugged in\n");
		exit(-1);
	}
	SwitchWrite(NOSERED);
	printf("Operate any switch to proceed\n");

	if (read(SwitchFD, buffer, sizeof(buffer)) < 0) {
		perror("Switch read: ");
		exit(3);
	}
	printf("Saitek Switch Panel ready\n");

	SwitchWrite(ALLOFF);
	int rval;

	rval = ((buffer[0] << 16) | (buffer[1] << 8) | (buffer[2]));

	return rval;
}

/************************************************************************************************
 * SwitchRead reads the Saitek Switch Panel, blocking read operation
 * returns an integer formed by the 3 bytes of data received packed into the l.s. bits
 * of the reply.
 * Data is described in 'SaitekProtocol'
 * **********************************************************************************************
 */
unsigned int SwitchRead(void) {

	if (read(SwitchFD, buffer, sizeof(buffer)) < 0) {
		perror("Switch read: ");
		exit(4);
	}
	return ((buffer[0] << 16) | (buffer[1] << 8) | (buffer[2]));
}

/***************************************************************************************
 * SwitchWrite
 * writes 'what' to the switch panel Landing Gear LEDS
 * *************************************************************************************
 */

void SwitchWrite(unsigned char what) {
	unsigned char buffer[2];
	buffer[0] = 0;
	buffer[1] = what;
	if (write(SwitchFD, buffer, 2) < 1) {
		perror("Write:");
		exit(5);
	}
}

