#/***********************************************************************/
#/* This file contains unpublished documentation and software           */
#/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
#/* in whole or in part, of the information in this file without a      */
#/* written consent of an officer of Cortina Systems Incorporated is    */
#/* strictly prohibited.                                                */
#/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
#/***********************************************************************/
#
# Makefile for building ringmac.o
#
# This file is released on Mon May  1 00:51:07 2006.
#

##############################################################
# The following 3 lines must be defined for each m2r Makefile
##############################################################
CHIP            =    MILAN2
CHIPROOT        =    $(shell cd ../..; pwd)
include $(CHIPROOT)/Makefile.common

##############################################################
# Local makefile options
##############################################################

# put the SRCS searching path here like PATH1:PATH1
VPATH           = $(M2RGENERALDIR):$(CSPLATFORMDIR) \
                 :$(M2RMODULEDIR)/XGXS:$(CHIPROOT)

# put the dependent header files here
HDRS            =    m2r_rmac.h m2r_rmac_priv.h		\
                     m2r_rmac_cam.h m2r_rmac_ms.h       \
                     m2r_rmac_rpr_fa.h                  \
                     m2r_rmac_srp_fa.h                  \
                     m2r_common.h m2r_public.h          \
                     cs_types.h cs_rtos.h m2r_error.h   \
                     m2r_registers.h m2r_rmac_mrw.h     \
                     m2r_rmac_irq.h			\
                     m2r_rmac_stats.h                   \
                     m2r_rmac_xgxs.h                    \
                     m2r_xgxs.h m2r_rmac_xgxs_stats.h   


INC            +=    -I.

# put all the source files here (without dir part)
SRCS            =    m2r_rmac.c 		       \
		     m2r_rmac_cfg.c		       \
                     m2r_rmac_cam.c                    \
                     m2r_rmac_ms.c		       \
                     m2r_rmac_ms_load.c                \
                     m2r_rmac_ctl.c                    \
                     m2r_rmac_mrw.c                    \
                     m2r_rmac_irq.c		       \
		     m2r_rmac_stats.c                  \
                     m2r_rmac_xgxs.c                   \
                     m2r_rmac_xgxs_stats.c

OBJS            =    $(SRCS:%.c=$(BUILDDIR)/%.o)


###########################################
# building target
###########################################
target: platform $(OBJS)

platform:
	$(CSPLATFORMDIR)/makebuilddir $(BUILDDIR)

$(BUILDDIR)/%.o : %.c $(HDRS)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) *.txt *.dat *.lst 
	$(RM) $(OBJS)

