#/***********************************************************************/
#/* This file contains unpublished documentation and software           */
#/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
#/* in whole or in part, of the information in this file without a      */
#/* written consent of an officer of Cortina Systems Incorporated is    */
#/* strictly prohibited.                                                */
#/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
#/***********************************************************************/
#
# Makefile for building general.o
#
# This file is released on Wed Nov 23 22:30:10 2005.
#

##############################################################
# The following 3 lines must be defined for each mdd Makefile
##############################################################
CHIP            =    ARSENAL5
CHIPROOT        =    $(shell cd ../..; pwd)
include $(CHIPROOT)/Makefile.common

##############################################################
# Local makefile options
##############################################################
VPATH           =   $(ARN5GENERALDIR):$(CSPLATFORMDIR) \
                   :$(CHIPROOT)
#VPATH 		=    $(CSPLATFORMDIR)

HDRS            =    cs_types.h cs_rtos.h	\
		     arn5_ver.h			\
		     arn5_error.h		\
		     arn5_cb.h			\
		     arn5_public.h		\
		     arn5_mpif_data.h		\
		     arn5_mpif_api.h		\
		     arn5_mpif_priv.h		\
		     arn5_mpif_irq.h		\
		     arn5_irq.h			\
		     arn5_irq_priv.h            \
		     arn5_common.h              \
		     arn5_debug.h               \
		     arn5_gen_api.h             \
		     arn5_gen_data.h            \
		     arn5_print.h               \
		     arn5_stats.h               \
		     arn5_stats_priv.h

INC	       +=    -I.

SRCS            =    arn5_drvr.c         \
                     arn5_dev.c		 \
		     arn5_port.c 	 \
		     arn5_error.c        \
                     arn5_mpif.c         \
                     arn5_stats.c        \
                     arn5_dev_stats.c    \
                     arn5_port_stats.c   \
		     arn5_debug.c 	 \
		     arn5_mpif_irq.c     \
		     arn5_irq.c

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
	$(RM) $(OBJS)

