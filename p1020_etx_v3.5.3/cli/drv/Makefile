################################################################################
# Makefile: 
#               top level makefile: use makefile in se7412x cards as reference
#               version: 01
################################################################################


QNF_HOME := $(shell pwd)

##########################################################################################
# Source Files
##########################################################################################
HYPHY20G_LIB := hyphy20g_lib/hyphy20g_hw.c hyphy20g_lib/hyphy20g_api.c hyphy20g_lib/hyphy20g_util_scbs.c \
	hyphy20g_lib/hyphy20g_util_ctxhdr.c \
	hyphy20g_lib/hyphy20g_rtos.c hyphy20g_lib/hyphy20g_cpb.c hyphy20g_lib/hyphy20g_indirect.c \
	hyphy20g_lib/hyphy20g_lsci.c hyphy20g_lib/hyphy20g_hsci.c \
	hyphy20g_lib/hyphy20g_otn.c hyphy20g_lib/hyphy20g_pif.c hyphy20g_lib/hyphy20g_mgmt.c \
	hyphy20g_lib/hyphy20g_util_ecom.c \
	hyphy20g_lib/hyphy20g_util_enet.c hyphy20g_lib/hyphy20g_lsci_status.c \
	hyphy20g_lib/hyphy20g_hsci_status.c \
	hyphy20g_lib/hyphy20g_pif_status.c hyphy20g_lib/hyphy20g_mgmt_status.c \
	hyphy20g_lib/hyphy20g_mgmt_counters.c \
	hyphy20g_lib/hyphy20g_lsci_counters.c hyphy20g_lib/hyphy20g_hsci_counters.c \
	hyphy20g_lib/hyphy20g_pif_counters.c \
	hyphy20g_lib/hyphy20g_sonet_ln.c hyphy20g_lib/hyphy20g_essi.c \
	hyphy20g_lib/hyphy20g_opi.c \
	hyphy20g_lib/hyphy20g_util_intlkn.c hyphy20g_lib/hyphy20g_dcsu.c \
	hyphy20g_lib/hyphy20g_gfp.c \
	hyphy20g_lib/hyphy20g_vcat.c hyphy20g_lib/hyphy20g_util_pmm.c \
	hyphy20g_lib/hyphy20g_util_acb.c \
	hyphy20g_lib/hyphy20g_bp_serdes.c hyphy20g_lib/hyphy20g_sonet_hopp.c \
	hyphy20g_lib/hyphy20g_sonet.c \
	hyphy20g_lib/hyphy20g_tse.c hyphy20g_lib/hyphy20g_dmi.c  hyphy20g_lib/hyphy20g_pcie.c \
	hyphy20g_lib/hyphy20g_sonet_status.c hyphy20g_lib/hyphy20g_sonet_features.c \
	hyphy20g_lib/hyphy20g_sonet_alarms.c \
	hyphy20g_lib/hyphy20g_serdes.c hyphy20g_lib/hyphy20g_hostmsg.c \
	hyphy20g_lib/hyphy20g_otn_counters.c hyphy20g_lib/hyphy20g_otn_ais.c \
	hyphy20g_lib/hyphy20g_otn_status.c hyphy20g_lib/hyphy20g_otn_overhead.c \
	hyphy20g_lib/hyphy20g_vcat_features.c \
	hyphy20g_lib/hyphy20g_vcat_status.c hyphy20g_lib/hyphy20g_essi_features.c \
	hyphy20g_lib/hyphy20g_essi_status.c \
	hyphy20g_lib/hyphy20g_loopbacks.c hyphy20g_lib/hyphy20g_gfp_features.c \
	hyphy20g_lib/hyphy20g_gfp_status.c \
	hyphy20g_lib/hyphy20g_hopp_status.c hyphy20g_lib/hyphy20g_hopp_features.c \
	hyphy20g_lib/hyphy20g_opi_features.c hyphy20g_lib/hyphy20g_dmi_status.c \
	hyphy20g_lib/hyphy20g_tse_status.c \
	hyphy20g_lib/hyphy20g_serdes_features.c hyphy20g_lib/hyphy20g_opi_counters.c \
	hyphy20g_lib/hyphy20g_otn_overhead_ram.c hyphy20g_lib/hyphy20g_tse_prgm.c \
	hyphy20g_lib/hyphy20g_pgmrclk.c hyphy20g_lib/hyphy20g_pif_features.c\
	hyphy20g_lib/hyphy20g_hsci_features.c hyphy20g_lib/hyphy20g_sonet_oh_ram.c \
	hyphy20g_lib/hyphy20g_lsci_features.c hyphy20g_lib/hyphy20g_hopp_alarms.c \
	hyphy20g_lib/hyphy20g_cpb_features.c hyphy20g_lib/hyphy20g_mgmt_features.c \
	hyphy20g_lib/hyphy20g_odukpp.c \
	hyphy20g_lib/hyphy20g_odukpp_status.c hyphy20g_lib/hyphy20g_odukpp_counters.c \
	hyphy20g_lib/hyphy20g_odukpp_overhead.c hyphy20g_lib/hyphy20g_odukpp_ais.c \
	hyphy20g_lib/hyphy20g_odukpp_overhead_ram.c \
	hyphy20g_lib/hyphy20g_wrapper.c

QNF_SRC	:= qnf_drv.c $(HYPHY20G_LIB)
TAP_SRC := tap_drv.c

EXTRA_CFLAGS += -DKERNEL_26X -D__KERNEL__ -D__linux__
EXTRA_CFLAGS +=-I$(QNF_HOME)/hyphy20g_lib

obj-m += qnf.o
ifeq ($(words $(findstring $(BOARDTYPE), $(BYPASS_BOARDS))), 1)
obj-m += tap.o
endif

qnf-objs = $(QNF_SRC:.c=.o)
ifeq ($(words $(findstring $(BOARDTYPE), $(BYPASS_BOARDS))), 1)
tap-objs = $(TAP_SRC:.c=.o)
endif

all:
	$(MAKE) -C $(KERNELDIR) CROSS_COMPILE=$(CROSS_COMPILE) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KERNELDIR) CROSS_COMPILE=$(CROSS_COMPILE) M=$(shell pwd) clean

