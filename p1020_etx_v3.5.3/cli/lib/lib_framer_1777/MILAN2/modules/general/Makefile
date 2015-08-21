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
VPATH 		=    $(CSPLATFORMDIR)

HDRS            =    cs_types.h cs_rtos.h m2r_error.h   \
                     m2r_cb.h m2r_common.h m2r_public.h \
		     m2r_gen.h m2r_irq.h m2r_stats.h    \
                     m2r_mpif.h

INC	       +=    -I.

SRCS            =    m2r_drvr.c                         \
                     m2r_dev.c                          \
                     m2r_port.c                         \
                     m2r_irq.c				\
		     m2r_debug.c                        \
                     m2r_stats.c			\
                     m2r_error.c                        \
                     m2r_mpif.c

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
