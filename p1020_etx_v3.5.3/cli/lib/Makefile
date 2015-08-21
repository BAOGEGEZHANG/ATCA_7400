################################################################################
# Makefile: 
#               top level makefile: use makefile in se7412x cards as reference
#               version: 01
#               version: 02
################################################################################

# libs for cs1331
FRAMER_LIB_1331_DIR = $(TOPDIR)/cli/lib/lib_framer_1331/ARSENAL5
ARN5MODULEDIR = $(FRAMER_LIB_1331_DIR)/modules
INC_1331 = -I$(FRAMER_LIB_1331_DIR)/../platform \
	  -I$(FRAMER_LIB_1331_DIR) \
	  -I$(ARN5MODULEDIR)/LIF  \
	  -I$(ARN5MODULEDIR)/Framer \
	  -I$(ARN5MODULEDIR)/ETH     \
	  -I$(ARN5MODULEDIR)/SPI	\
	  -I$(ARN5MODULEDIR)/PPRBS \
	  -I$(ARN5MODULEDIR)/general \
	  -I../include
ARN5FLAGS = -DRELEASE_PLATFORM -DARSENAL5

# libs for cs1777
##FRAMER_LIB_1777_DIR = $(TOPDIR)/cli/lib/lib_framer_1777/MILAN2
##M2RMODULEDIR = $(FRAMER_LIB_1777_DIR)/modules
##INC_1777 = -I$(FRAMER_LIB_1777_DIR)/../platform \
##	  -I$(FRAMER_LIB_1777_DIR) \
##	  -I$(M2RMODULEDIR)/general \
##	  -I$(M2RMODULEDIR)/SPI42	\
##	  -I$(M2RMODULEDIR)/PPRBS \
##	  -I$(M2RMODULEDIR)/Framer \
##	  -I$(M2RMODULEDIR)/RingMAC \
##	  -I$(M2RMODULEDIR)/XGXS    \
##	  -I$(M2RMODULEDIR)/ETH     \
##	  -I$(M2RMODULEDIR)/LIF  \
##	  -I../include
##M2RFLAGS = -DRELEASE_PLATFORM -DMILAN2


CFLAGS += -D__linux__ -DLinux -DQNF_BIG_ENDIAN -DCS_BIG_ENDIAN

##.PHONY:  all libqnfapi.a qnfapi.o arsenal5.o milan2.o clean
.PHONY:  all libqnfapi.a qnfapi.o arsenal5.o clean

all: libqnfapi.a


libqnfapi.a: arsenal5.o qnfapi.o
	$(AR) rcs libqnfapi.a $(FRAMER_LIB_1331_DIR)/targets/Linux/arsenal5.o qnfapi.o

arsenal5.o:
	$(MAKE) -C $(FRAMER_LIB_1331_DIR)

##milan2.o:
##	$(MAKE) -C $(FRAMER_LIB_1777_DIR)

qnfapi.o:
	$(CC) -c -O2 -g qnfapi.c $(CFLAGS) $(TARGET_APP_FLAGS) $(ARN5FLAGS) $(INC_1331) -o qnfapi.o


clean:
	$(RM) *.o *.a
	$(MAKE) -C $(FRAMER_LIB_1331_DIR) cleanall
##	$(MAKE) -C $(FRAMER_LIB_1777_DIR) cleanall

