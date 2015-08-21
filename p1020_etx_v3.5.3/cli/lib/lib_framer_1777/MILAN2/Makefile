#
# Makefile for Milan2 headers
#
# This file is released on Mon May  1 00:51:07 2006.
#

export CHIP     =    MILAN2
CHIPROOT        =    $(shell pwd)
include Makefile.common

##############################################################
# Local makefile options
##############################################################

TARGET          =    $(BUILDDIR)/libmilan2.a
OBJS            =    $(BUILDDIR)/m2r_*.o

###########################################
# building target
###########################################

all: m2r_registers.h $(CSPLATFORMDIR)/makebuilddir
	$(CSPLATFORMDIR)/makebuilddir $(BUILDDIR)
	$(MAKE) -e -C $(M2RMODULEDIR)/Framer
	$(MAKE) -e -C $(M2RMODULEDIR)/LIF
	$(MAKE) -e -C $(M2RMODULEDIR)/PPRBS
	$(MAKE) -e -C $(M2RMODULEDIR)/RingMAC
	$(MAKE) -e -C $(M2RMODULEDIR)/SPI42
	$(MAKE) -e -C $(M2RMODULEDIR)/XGXS
	$(MAKE) -e -C $(M2RMODULEDIR)/ETH
	$(MAKE) -e -C $(M2RMODULEDIR)/general
	$(AR) -r $(TARGET) $(OBJS)
	$(LD) -r $(OBJS) -o $(BUILDDIR)/milan2.o 


cleanall:
	$(MAKE) -e -C $(M2RMODULEDIR)/Framer clean
	$(MAKE) -e -C $(M2RMODULEDIR)/LIF clean
	$(MAKE) -e -C $(M2RMODULEDIR)/PPRBS clean
	$(MAKE) -e -C $(M2RMODULEDIR)/RingMAC clean
	$(MAKE) -e -C $(M2RMODULEDIR)/SPI42 clean
	$(MAKE) -e -C $(M2RMODULEDIR)/XGXS clean
	$(MAKE) -e -C $(M2RMODULEDIR)/ETH clean
	$(MAKE) -e -C $(M2RMODULEDIR)/general clean
	$(RM) $(TARGET)
	$(RM) $(BUILDDIR)/milan2.o



