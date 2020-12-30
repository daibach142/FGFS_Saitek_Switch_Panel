#
# Makefile to generate SimulatorPanel.h from specified aircraft XML file
#
# Typically, AIRCRAFT in environment variable (set by ./configure)
#
.PHONY: all

all:
	xsltproc --stringparam current-date `date +%Y-%m-%d` toHtml.xsl $(AIRCRAFT).xml >../src/SimulatorPanel.html
	xsltproc --stringparam current-date `date +%Y-%m-%d` --stringparam current-year `date +%Y` \
	toInclude.xsl $(AIRCRAFT).xml >../src/SimulatorPanel.h
	

