#
# Makefile for Arsenal5 headers
#
# This file is released on Wed Nov 23 22:30:10 2005.
#

export CHIP     =    ARSENAL5
CHIPROOT        =    $(shell pwd)
include Makefile.common

##############################################################
# Local makefile options
##############################################################

TARGET          =    $(BUILDDIR)/libarsenal5.a
OBJS            =    $(BUILDDIR)/arn5_*.o

###########################################
# building target
###########################################

all: arn5_registers.h $(CSPLATFORMDIR)/makebuilddir
	$(CSPLATFORMDIR)/makebuilddir $(BUILDDIR)
	$(MAKE) -e -C $(ARN5MODULEDIR)/LIF
	$(MAKE) -e -C $(ARN5MODULEDIR)/Framer
	$(MAKE) -e -C $(ARN5MODULEDIR)/ETH
	$(MAKE) -e -C $(ARN5MODULEDIR)/SPI
	$(MAKE) -e -C $(ARN5MODULEDIR)/PPRBS
	$(MAKE) -e -C $(ARN5MODULEDIR)/general
	$(AR) -r $(TARGET) $(OBJS)
	$(LD) -r $(OBJS) -o $(BUILDDIR)/arsenal5.o


cleanall:
	$(MAKE) -e -C $(ARN5MODULEDIR)/LIF clean
	$(MAKE) -e -C $(ARN5MODULEDIR)/Framer clean
	$(MAKE) -e -C $(ARN5MODULEDIR)/ETH clean
	$(MAKE) -e -C $(ARN5MODULEDIR)/SPI clean
	$(MAKE) -e -C $(ARN5MODULEDIR)/PPRBS clean
	$(MAKE) -e -C $(ARN5MODULEDIR)/general clean
	$(RM) $(TARGET)
	$(RM) $(BUILDDIR)/arsenal5.o

