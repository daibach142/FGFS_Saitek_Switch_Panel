# Simple makefile
#
# something to run src/Makefile

-include Configparams

.PHONY: install clean uninstall executables

executables: Configparams
	cd Generators && $(MAKE)
	cd src && $(MAKE)

install:
	cp src/SaitekSwitchPanel /usr/games
	cp StartSwitchPanel /usr/games
	cp Extras/55-saitek.rules /etc/udev/rules.d
	cp Extras/55-saitekpanels.conf /usr/share/X11/xorg.conf.d
	cp Extras/stkswitch_input.xml /usr/share/games/flightgear/Protocol
	cp Extras/Switch.nas ~/.fgfs/Nasal
	udevadm control --reload

clean:
	rm Configparams src/SaitekSwitchPanel StartSwitchPanel

uninstall:
	rm /usr/games/SaitekSwitchPanel 
	rm /usr/games/StartSwitchPanel
	rm /etc/udev/rules.d/55-saitek.rules
	rm /usr/share/X11/xorg.conf.d/55-saitekpanels.conf
	udevadm control --reload
	-pkill -9 StartSwitchPanel
	-pkill -9 SaitekSwitchPane


Configparams:
	$(error Run ./configure first)




