##################################################################################
# 
# Version 3.5: include BOARDTYPE define
# 2014.05.19
# 
##################################################################################


TOPDIR = $(shell pwd)
KERNELDIR = /home/cvspub/kernel

CROSS_COMPILE = powerpc-fsl-linux-gnuspe-
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
LD = $(CROSS_COMPILE)ld
STRIP = $(CROSS_COMPILE)strip

AWK = awk
PWD = /bin/pwd
RM = /bin/rm -f
CP = /bin/cp -f


MODULE_FLAGS = -D__KERNEL__ -Wall -Wno-strict-prototypes -Wno-trigraphs -O2 \
	-fomit-frame-pointer \
	-I$(KERNELDIR)/arch/ppc -fsigned-char -pipe \
	-ffixed-r2 -Wno-uninitialized -mmultiple -mstring
APP_FLAGS = -Wall -Wno-strict-prototypes -O2 -D_REENTRANT

COMMON_FLAGS = -D_LINUX -msoft-float -fno-strict-aliasing -I$(TOPDIR)

DEFINES= -I$(KERNELDIR)/include 
UDEFINES= 

#define for different boards
BOARDTYPE = 
BUILDVER = -DBUILD_$(BOARDTYPE)

TARGET_MODULE_FLAGS += $(COMMON_FLAGS) $(DEFINES) $(MODULE_FLAGS) $(BUILDVER)
TARGET_APP_FLAGS += $(COMMON_FLAGS) $(APP_FLAGS) $(BUILDVER)

BYPASS_BOARDS := QT10 QT20


export TOPDIR KERNELDIR
export CC AR LD STRIP
export AWK PWD RM CP
export TARGET_MODULE_FLAGS TARGET_APP_FLAGS
export BOARDTYPE BYPASS_BOARDS


.PHONY: all bootsetup cs_server db_server cli clean

all: cli bootsetup db_server cs_server


#target app order is important
bootsetup:
	$(MAKE) -C $(TOPDIR)/bootsetup
	$(CP) $(TOPDIR)/bootsetup/$@ $(TOPDIR)/targets/sbin/.
	
db_server:
	$(MAKE) -C $(TOPDIR)/db_server
	$(CP) $(TOPDIR)/db_server/$@ $(TOPDIR)/targets/sbin/.

cs_server:
	$(MAKE) -C $(TOPDIR)/cs_server
	$(CP) $(TOPDIR)/cs_server/$@ $(TOPDIR)/targets/sbin/.

cli:
	$(MAKE) -C $(TOPDIR)/cli
	$(CP) $(TOPDIR)/cli/cli $(TOPDIR)/targets/bin/.
	$(CP) $(TOPDIR)/cli/cli_demo $(TOPDIR)/targets/sbin/.
ifeq ($(words $(findstring $(BOARDTYPE), $(BYPASS_BOARDS))), 1)
	$(CP) $(TOPDIR)/cli/bypass_monitor $(TOPDIR)/targets/sbin/.
	$(CP) $(TOPDIR)/cli/drv/tap.ko $(TOPDIR)/targets/modules/.
endif
# snmp_monitor is removed from SDK6.3.2	
#	$(CP) $(TOPDIR)/cli/snmp_monitor $(TOPDIR)/targets/sbin/.
	$(CP) $(TOPDIR)/cli/drv/qnf.ko $(TOPDIR)/targets/modules/.

clean:
	$(RM) $(TOPDIR)/targets/modules/qnf.ko
	$(RM) $(TOPDIR)/targets/sbin/bootsetup
	$(RM) $(TOPDIR)/targets/sbin/db_server
	$(RM) $(TOPDIR)/targets/sbin/cs_server
# snmp_monitor is removed from SDK6.3.2	
#	$(RM) $(TOPDIR)/targets/sbin/snmp_monitor
ifeq ($(words $(findstring $(BOARDTYPE), $(BYPASS_BOARDS))), 1)
	$(RM) $(TOPDIR)/targets/sbin/bypass_monitor
	$(RM) $(TOPDIR)/targets/modules/tap.ko
endif
	$(RM) $(TOPDIR)/targets/sbin/cli_demo
	$(RM) $(TOPDIR)/targets/bin/cli
	$(MAKE) -C $(TOPDIR)/bootsetup clean
	$(MAKE) -C $(TOPDIR)/db_server clean
	$(MAKE) -C $(TOPDIR)/cs_server clean
	$(MAKE) -C $(TOPDIR)/cli clean

