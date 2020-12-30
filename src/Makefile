#
# make saitek switch panel 
#

.PHONY: all

all:
ifeq (LIBUDEV, 1)
	gcc -D HAVE_LIBUDEV_H *.c -o SaitekSwitchPanel -ludev -O3
else
	gcc *.c -o SaitekSwitchPanel -O3
endif

